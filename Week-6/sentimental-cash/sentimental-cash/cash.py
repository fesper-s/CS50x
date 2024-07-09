from cs50 import get_float


def main():
    change = -1
    while change < 0:
        change = get_float("Change: ")

    change = change * 100

    coins = 0
    while change > 0:
        if change >= 25:
            change -= 25
        elif change >= 10:
            change -= 10
        elif change >= 5:
            change -= 5
        else:
            change -= 1
        coins += 1

    print(coins)


main()
