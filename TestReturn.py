from time import sleep

if __name__ == '__main__':
    counter = 0

    while True:
        counter += 1
        counter_str = str(counter)
        print("This Will Remain Still")
        print("Counter is " + counter_str,end='\033[F')
        sleep(1)
