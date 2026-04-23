/*
 * ============================================================
 *   REAL-TIME STOCK ORDER MATCHING ENGINE
 *   Language : C++17
 *   DSA Used : Priority Queue (Max-Heap / Min-Heap), Hash Map,
 *              Linked List (trade log), Queue (FIFO orders)
 * ============================================================
 *
 *  HOW IT WORKS
 *  ------------
 *  Buy  orders are stored in a MAX-HEAP  (highest price first).
 *  Sell orders are stored in a MIN-HEAP  (lowest  price first).
 *  Whenever the best buy price >= best sell price, a TRADE is executed.
 *  Partial fills are supported: the remainder stays in the book.
 *  All placed / cancelled / filled orders are tracked via an unordered_map.
 */

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

// ─────────────────────────────────────────────
//  ENUMS
// ─────────────────────────────────────────────
enum class Side
{
    BUY,
    SELL
};
enum class Status
{
    OPEN,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

std::string sideStr(Side s) { return s == Side::BUY ? "BUY" : "SELL"; }
std::string statusStr(Status s)
{
    switch (s)
    {
    case Status::OPEN:
        return "OPEN";
    case Status::PARTIALLY_FILLED:
        return "PARTIALLY_FILLED";
    case Status::FILLED:
        return "FILLED";
    case Status::CANCELLED:
        return "CANCELLED";
    }
    return "";
}

// ─────────────────────────────────────────────
//  ORDER
// ─────────────────────────────────────────────
struct Order
{
    int id;
    std::string symbol;
    Side side;
    double price;
    int quantity;
    int filledQty = 0;
    Status status = Status::OPEN;
    long long timestamp; // epoch ms

    int remainingQty() const { return quantity - filledQty; }
};

// ─────────────────────────────────────────────
//  TRADE RECORD
// ─────────────────────────────────────────────
struct Trade
{
    int tradeId;
    std::string symbol;
    int buyOrderId;
    int sellOrderId;
    double price;
    int quantity;
    long long timestamp;
};

// ─────────────────────────────────────────────
//  COMPARATORS FOR PRIORITY QUEUES
// ─────────────────────────────────────────────
// Buy  side: highest price first (MAX-HEAP); on tie, earlier order first
struct BuyComparator
{
    bool operator()(const Order *a, const Order *b) const
    {
        if (a->price != b->price)
            return a->price < b->price;
        return a->timestamp > b->timestamp; // earlier = higher priority
    }
};
// Sell side: lowest price first (MIN-HEAP); on tie, earlier order first
struct SellComparator
{
    bool operator()(const Order *a, const Order *b) const
    {
        if (a->price != b->price)
            return a->price > b->price;
        return a->timestamp > b->timestamp;
    }
};

using BuyBook = std::priority_queue<Order *, std::vector<Order *>, BuyComparator>;
using SellBook = std::priority_queue<Order *, std::vector<Order *>, SellComparator>;

// ─────────────────────────────────────────────
//  ORDER BOOK  (per symbol)
// ─────────────────────────────────────────────
struct OrderBook
{
    BuyBook bids;
    SellBook asks;
};

// ─────────────────────────────────────────────
//  MATCHING ENGINE
// ─────────────────────────────────────────────
class MatchingEngine
{
public:
    // Place a new limit order
    int placeOrder(const std::string &symbol, Side side,
                   double price, int quantity)
    {
        int id = ++nextOrderId;
        Order *o = new Order{id, symbol, side, price, quantity, 0,
                             Status::OPEN, nowMs()};
        orders[id] = o;

        std::cout << "\n[ORDER PLACED] #" << id
                  << " " << sideStr(side)
                  << " " << quantity << " " << symbol
                  << " @ " << std::fixed << std::setprecision(2) << price << "\n";

        match(symbol, o);
        return id;
    }

    // Cancel an open order
    bool cancelOrder(int orderId)
    {
        auto it = orders.find(orderId);
        if (it == orders.end())
        {
            std::cout << "[ERROR] Order #" << orderId << " not found.\n";
            return false;
        }
        Order *o = it->second;
        if (o->status != Status::OPEN && o->status != Status::PARTIALLY_FILLED)
        {
            std::cout << "[ERROR] Order #" << orderId
                      << " cannot be cancelled (status: " << statusStr(o->status) << ")\n";
            return false;
        }
        o->status = Status::CANCELLED;
        std::cout << "[CANCELLED] Order #" << orderId << "\n";
        return true;
    }

    // Print the order book for a symbol
    void printBook(const std::string &symbol) const
    {
        std::cout << "\n══════════  ORDER BOOK: " << symbol << "  ══════════\n";

        // Collect active orders
        // Collect active orders
        std::vector<Order *> bids, asks;
        for (auto &pair : orders)
        {
            int id = pair.first;
            Order *o = pair.second;

            if (o->symbol != symbol)
                continue;
            if (o->status != Status::OPEN && o->status != Status::PARTIALLY_FILLED)
                continue;
            if (o->side == Side::BUY)
                bids.push_back(o);
            else
                asks.push_back(o);
        }
        std::sort(bids.begin(), bids.end(),
                  [](Order *a, Order *b)
                  { return a->price > b->price; });
        std::sort(asks.begin(), asks.end(),
                  [](Order *a, Order *b)
                  { return a->price < b->price; });

        std::cout << std::left
                  << std::setw(8) << "ID"
                  << std::setw(8) << "SIDE"
                  << std::setw(10) << "PRICE"
                  << std::setw(8) << "QTY"
                  << std::setw(8) << "FILLED"
                  << "STATUS\n";
        std::cout << std::string(54, '-') << "\n";

        for (Order *o : asks)
            printOrderRow(o);
        std::cout << std::string(54, '-') << "  <-- SPREAD\n";
        for (Order *o : bids)
            printOrderRow(o);
        std::cout << "\n";
    }

    // Print all executed trades
    void printTrades() const
    {
        std::cout << "\n══════════  TRADE LOG  ══════════\n";
        if (trades.empty())
        {
            std::cout << "  (no trades yet)\n\n";
            return;
        }
        std::cout << std::left
                  << std::setw(8) << "TRADE#"
                  << std::setw(8) << "SYMBOL"
                  << std::setw(8) << "BUY#"
                  << std::setw(8) << "SELL#"
                  << std::setw(10) << "PRICE"
                  << "QTY\n";
        std::cout << std::string(50, '-') << "\n";
        for (const Trade &t : trades)
        {
            std::cout << std::setw(8) << t.tradeId
                      << std::setw(8) << t.symbol
                      << std::setw(8) << t.buyOrderId
                      << std::setw(8) << t.sellOrderId
                      << std::setw(10) << std::fixed << std::setprecision(2) << t.price
                      << t.quantity << "\n";
        }
        std::cout << "\n";
    }

    // Query a single order
    void queryOrder(int id) const
    {
        auto it = orders.find(id);
        if (it == orders.end())
        {
            std::cout << "[ERROR] Order #" << id << " not found.\n";
            return;
        }
        Order *o = it->second;
        std::cout << "[ORDER #" << id << "] "
                  << o->symbol << " " << sideStr(o->side)
                  << " @ " << o->price
                  << "  Qty=" << o->quantity
                  << "  Filled=" << o->filledQty
                  << "  Status=" << statusStr(o->status) << "\n";
    }

    ~MatchingEngine()
    {
        for (auto &pair : orders)
        {
            delete pair.second;
        }
    }

private:
    int nextOrderId = 0;
    int nextTradeId = 0;
    std::unordered_map<std::string, OrderBook> books;
    std::unordered_map<int, Order *> orders;
    std::vector<Trade> trades;

    long long nowMs()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

    void match(const std::string &symbol, Order *incoming)
    {
        OrderBook &book = books[symbol];

        if (incoming->side == Side::BUY)
        {
            book.bids.push(incoming);
        }
        else
        {
            book.asks.push(incoming);
        }

        // Try to match top of book
        while (!book.bids.empty() && !book.asks.empty())
        {
            Order *bestBuy = book.bids.top();
            Order *bestSell = book.asks.top();

            // Skip cancelled orders
            if (bestBuy->status == Status::CANCELLED)
            {
                book.bids.pop();
                continue;
            }
            if (bestSell->status == Status::CANCELLED)
            {
                book.asks.pop();
                continue;
            }

            if (bestBuy->price < bestSell->price)
                break; // No match

            // ── TRADE EXECUTION ──
            int tradeQty = std::min(bestBuy->remainingQty(),
                                    bestSell->remainingQty());
            double tradePrice = bestSell->price; // price-time priority: seller's price

            bestBuy->filledQty += tradeQty;
            bestSell->filledQty += tradeQty;

            bestBuy->status = (bestBuy->remainingQty() == 0) ? Status::FILLED
                                                             : Status::PARTIALLY_FILLED;
            bestSell->status = (bestSell->remainingQty() == 0) ? Status::FILLED
                                                               : Status::PARTIALLY_FILLED;

            Trade t{++nextTradeId, symbol, bestBuy->id, bestSell->id,
                    tradePrice, tradeQty, nowMs()};
            trades.push_back(t);

            std::cout << "  ✔ TRADE #" << t.tradeId
                      << " | " << symbol
                      << " | Buy#" << bestBuy->id
                      << " <-> Sell#" << bestSell->id
                      << " | " << tradeQty << " @ "
                      << std::fixed << std::setprecision(2) << tradePrice << "\n";

            if (bestBuy->status == Status::FILLED)
                book.bids.pop();
            if (bestSell->status == Status::FILLED)
                book.asks.pop();
        }
    }

    void printOrderRow(Order *o) const
    {
        std::cout << std::setw(8) << o->id
                  << std::setw(8) << sideStr(o->side)
                  << std::setw(10) << std::fixed << std::setprecision(2) << o->price
                  << std::setw(8) << o->quantity
                  << std::setw(8) << o->filledQty
                  << statusStr(o->status) << "\n";
    }
};

// ─────────────────────────────────────────────
//  MAIN  –  demonstration scenario
// ─────────────────────────────────────────────
int main()
{
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║   REAL-TIME STOCK ORDER MATCHING ENGINE      ║\n";
    std::cout << "║   DSA: Heap, HashMap, Vector, Priority Queue ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";

    MatchingEngine engine;

    // ── SCENARIO 1: Basic full match ──────────────
    std::cout << "\n\n━━━ SCENARIO 1 : Full Match ━━━\n";
    engine.placeOrder("RELIANCE", Side::SELL, 2500.00, 100);
    engine.placeOrder("RELIANCE", Side::BUY, 2500.00, 100);
    engine.printBook("RELIANCE");

    // ── SCENARIO 2: Partial fill ──────────────────
    std::cout << "\n━━━ SCENARIO 2 : Partial Fill ━━━\n";
    engine.placeOrder("TCS", Side::SELL, 3400.00, 200);
    engine.placeOrder("TCS", Side::BUY, 3400.00, 80);
    engine.printBook("TCS");

    // ── SCENARIO 3: Price-time priority ──────────
    std::cout << "\n━━━ SCENARIO 3 : Price-Time Priority ━━━\n";
    engine.placeOrder("INFY", Side::SELL, 1500.00, 50);
    engine.placeOrder("INFY", Side::SELL, 1510.00, 50);
    engine.placeOrder("INFY", Side::SELL, 1490.00, 50); // best ask
    engine.placeOrder("INFY", Side::BUY, 1510.00, 120); // should match 1490 then 1500 then 1510
    engine.printBook("INFY");

    // ── SCENARIO 4: Cancellation ──────────────────
    std::cout << "\n━━━ SCENARIO 4 : Order Cancellation ━━━\n";
    int oid = engine.placeOrder("WIPRO", Side::BUY, 450.00, 300);
    engine.cancelOrder(oid);
    engine.queryOrder(oid);

    // ── FULL TRADE LOG ────────────────────────────
    engine.printTrades();

    return 0;
}

/*
 * ─────────────────────────────────────────────────────────────
 *  DSA CONCEPTS USED — QUICK REFERENCE
 * ─────────────────────────────────────────────────────────────
 *
 *  1. PRIORITY QUEUE (Binary Heap)
 *     • Buy  side: Max-Heap  → O(log n) insert / O(log n) extract-max
 *     • Sell side: Min-Heap  → O(log n) insert / O(log n) extract-min
 *     • Enables O(log n) matching per order
 *
 *  2. HASH MAP  (std::unordered_map)
 *     • Order lookup by ID        → O(1) average
 *     • Symbol → OrderBook map    → O(1) average
 *
 *  3. VECTOR  (std::vector<Trade>)
 *     • Append-only trade log     → O(1) amortised push_back
 *
 *  4. COMPARATOR / FUNCTOR
 *     • Custom comparators implement price-time priority
 *
 *  OVERALL COMPLEXITY
 *     Place order  : O(log n)   — n = orders in book
 *     Cancel order : O(1)       — lazy deletion via status flag
 *     Query order  : O(1)
 *     Print book   : O(n log n) — sort for display only
 * ─────────────────────────────────────────────────────────────
 */