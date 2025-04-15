#! /usr/bin/env python3
import threading
import time

class RW:
    def __init__(self):
        self.readers = 0
        self.writing = False
        self.wpending = 0
        self.cond = threading.Condition()

    # Reader lock
    def read_lock(self):
        tid = threading.get_native_id() % 100
        print(f"{tid:2d} read request")
        with self.cond:
            while self.writing or self.wpending > 0:
                self.cond.wait()
            self.readers += 1

    # Reader unlock
    def read_unlock(self):
        tid = threading.get_native_id() % 100
        print(f"{tid:2d} read completed")
        with self.cond:
            assert self.readers > 0  # There must be at least one reader
            assert not self.writing  # There must not be any writer
            self.readers -= 1
            if self.readers == 0:
                self.cond.notify()

    # Writer lock
    def write_lock(self):
        tid = threading.get_native_id() % 100
        print(f"{tid:2d} write request")
        with self.cond:
            self.wpending += 1
            while self.writing or self.readers > 0:
                self.cond.wait()
            self.writing = True
            self.wpending -= 1

    # Writer unlock
    def write_unlock(self):
        tid = threading.get_native_id() % 100
        print(f"{tid:2d} write completed")
        with self.cond:
            assert self.writing
            self.writing = False
            self.cond.notify_all()

# Reader thread function
def lettore(rw_obj):
    rw_obj.read_lock()
    time.sleep(1)
    rw_obj.read_unlock()

# Writer thread function
def scrittore(rw_obj):
    rw_obj.write_lock()
    time.sleep(1)
    rw_obj.write_unlock()

def main():
    rw_obj = RW()
    threads = []

    # Create threads
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=scrittore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=scrittore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))
    threads.append(threading.Thread(target=lettore, args=(rw_obj,)))

    # Start threads
    for t in threads:
        t.start()

    # Wait for all threads to finish
    for t in threads:
        t.join()

    # Final assertions
    assert rw_obj.wpending == 0
    assert not rw_obj.writing
    assert rw_obj.readers == 0

if __name__ == "__main__":
    main()
