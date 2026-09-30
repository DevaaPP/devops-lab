try:
    from calculator import add, substract
except ImportError:
    def add(a, b):
        return a + b

    def substract(a, b):
        return a - b


if __name__ == "__main__":
    print(add(2, 3), substract(5, 4))