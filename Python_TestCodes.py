while True: 
    password = input("Enter your password:")

    has_letter = False
    has_number = False

    for ch in password:
        if ch.isalpha():
            has_letter = True 
        if ch.isdigit():
            has_number = True

    if has_letter and has_number:
        print("Password Granted")
        break
    else:
        print("Password Denied. Try Again")
        print()
        continue 