from datetime import datetime
import sys

class GroceryBuddy:
    def __init__(self):
        self.basket = []
        self.prices = {}
        self.total = 0.0
        self.session_start = datetime.now()
    
    def add_groceries(self, product, cost=0.0):
        """Add item with optional price tracking"""
        entry = {
            'name': product,
            'price': cost,
            'timestamp': datetime.now().strftime("%H:%M:%S")
        }
        self.basket.append(entry)
        self.prices[product] = cost
        self.total += cost
        print(f"✨ '{product}' added to your basket ${cost:.2f}")
    
    def discard_item(self, product):
        """Remove item if it exists"""
        for idx, item in enumerate(self.basket):
            if item['name'].lower() == product.lower():
                removed = self.basket.pop(idx)
                self.total -= removed['price']
                print(f"🗑️  '{product}' removed from basket")
                return
        print(f"❌ '{product}' not found in basket")
    
    def show_inventory(self):
        """Display current basket contents"""
        if not self.basket:
            print("\n╔════════════════════════╗")
            print("║    🛒 Basket Empty     ║")
            print("╚════════════════════════╝")
            return
        
        print("\n" + "═" * 50)
        print("              YOUR SHOPPING BASKET")
        print("═" * 50)
        print(f"{'#':<3} {'ITEM':<20} {'PRICE':<10} {'TIME':<8}")
        print("─" * 50)
        
        for idx, grocery in enumerate(self.basket, 1):
            print(f"{idx:<3} {grocery['name']:<20} "
                  f"${grocery['price']:<9.2f} {grocery['timestamp']:<8}")
        
        print("─" * 50)
        print(f"{'TOTAL':<24} ${self.total:<9.2f}")
        print("═" * 50)
    
    def purchase_summary(self):
        """Final checkout with receipt"""
        print("\n" + "🎉" * 15)
        print("     CHECKOUT COMPLETE")
        print("🎉" * 15)
        
        if self.basket:
            print("\nYOUR RECEIPT:")
            print("-" * 30)
            for item in self.basket:
                print(f"• {item['name']:<15} ${item['price']:.2f}")
            print("-" * 30)
            print(f"GRAND TOTAL: ${self.total:.2f}")
            print(f"ITEMS: {len(self.basket)}")
        else:
            print("Nothing purchased this time!")
        
        session_duration = datetime.now() - self.session_start
        print(f"\nShopping time: {session_duration.seconds // 60}m {session_duration.seconds % 60}s")
        print("Thank you for shopping with GroceryBuddy!")


def fancy_divider(char="★", length=40):
    """Create a decorative divider"""
    print(char * length)


# Main program
app = GroceryBuddy()
active_session = True

while active_session:
    fancy_divider("─")
    print("""
    ╔══════════════════════════╗
    ║    🛍️  MARKET MATE       ║
    ╠══════════════════════════╣
    ║  [A] ➕ Add Item         ║
    ║  [R] ➖ Remove Item      ║
    ║  [V] 👁️  View Basket     ║
    ║  [C] 💳 Checkout         ║
    ║  [Q] 🚪 Quit             ║
    ╚══════════════════════════╝
    """)
    
    user_input = input("⚡ Select option: ").strip().upper()
    
    match user_input:
        case "A":
            product_name = input("📦 Product name: ").strip()
            if product_name:
                try:
                    item_price = float(input("💰 Price (optional, 0 if unknown): ") or "0")
                    app.add_groceries(product_name, item_price)
                except ValueError:
                    print("⚠️  Invalid price, using $0.00")
                    app.add_groceries(product_name)
            else:
                print("⚠️  Product name cannot be empty!")
        
        case "R":
            if not app.basket:
                print("📭 Basket is already empty!")
            else:
                app.show_inventory()
                target_item = input("🗑️  Item to remove: ").strip()
                app.discard_item(target_item)
        
        case "V":
            app.show_inventory()
        
        case "C":
            if app.basket:
                app.purchase_summary()
                print("\nStarting new basket...")
                app = GroceryBuddy()  # Reset for next customer
            else:
                print("😕 Nothing to checkout yet!")
        
        case "Q":
            if app.basket:
                print("\n⚠️  You have items in your basket!")
                confirm = input("Are you sure you want to quit? (Y/N): ").strip().upper()
                if confirm == "Y":
                    print("👋 See you next time!")
                    active_session = False
            else:
                print("👋 Thanks for stopping by!")
                active_session = False
        
        case _:
            print("❌ Invalid option! Please try again.")
    
    if active_session:
        input("\n⏎ Press Enter to continue...")

print("\n" + "⭐" * 20)
print("  Session ended")
print("⭐" * 20)