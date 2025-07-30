#!/usr/bin/env python3

def detect_with_proc():
    """Parse /proc/asound/cards to get basic card info without using regex."""
    cards = {}
    try:
        with open('/proc/asound/cards') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("---"):
                    continue  # skip empty lines and separators

                parts = line.split('[', 1)
                if len(parts) < 2:
                    continue

                idx_and_pre = parts[0].strip().split()
                if not idx_and_pre or not idx_and_pre[0].isdigit():
                    continue

                index = int(idx_and_pre[0])

                rest = parts[1].split(']:', 1)
                if len(rest) < 2:
                    continue

                tag = rest[0].strip()
                desc = rest[1].strip()
                cards[index] = f"{tag}: {desc}"
    except FileNotFoundError:
        print("No /proc/asound/cards found. Is ALSA loaded?")
    return cards

def main():
    print("Detected ALSA devices via /proc/asound/cards:\n")
    cards = detect_with_proc()
    for idx, desc in cards.items():
        print(f"Card {idx}: {desc}")

if __name__ == '__main__':
    main()
