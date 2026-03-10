import operator

def calculate(operation, x, y):
    operations = {
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
        '^': operator.pow,
        '%': operator.mod
    }
    return operations.get(operation, lambda a, b: "Invalid operation")(x, y)

def get_number(prompt):
    while True:
        try:
            return float(input(prompt))
        except ValueError:
            print("Please enter a valid number!")

def display_menu():
    menu = """
╔════════════════════╗
║   MATH ASSISTANT   ║
╠════════════════════╣
║  +  Addition       ║
║  -  Subtraction    ║
║  *  Multiplication ║
║  /  Division       ║
║  ^  Power          ║
║  %  Modulus        ║
║  q  Quit           ║
╚════════════════════╝
"""
    print(menu)

active = True
history = []

while active:
    display_menu()
    user_choice = input("Enter operation: ").strip().lower()
    
    if user_choice == 'q':
        print("\n👋 Thanks for calculating! Goodbye!")
        if history:
            print("\nYour calculation history:")
            for idx, entry in enumerate(history, 1):
                print(f"  {idx}. {entry}")
        active = False
        continue
    
    if user_choice in ['+', '-', '*', '/', '^', '%']:
        value_a = get_number("First number: ")
        value_b = get_number("Second number: ")
        
        if user_choice == '/' and value_b == 0:
            print("❌ Error: Cannot divide by zero!")
        else:
            result = calculate(user_choice, value_a, value_b)
            equation = f"{value_a} {user_choice} {value_b} = {result}"
            print(f"\n✅ Result: {equation}")
            history.append(equation)
    else:
        print("❌ Invalid choice! Please select a valid operation.")
    
    input("\nPress Enter to continue...")