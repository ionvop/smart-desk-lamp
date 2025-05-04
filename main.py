import webview
import cv2
import base64
import requests
import json
import time


config = json.load(open("config.json"))
cap = cv2.VideoCapture(config["camera_ip"] + ":81/stream")
latest_frame = None
is_running = True
test = 0


class API:
    def get_frame(self):
        global test
        _, img_encoded = cv2.imencode(".jpg", cv2.flip(latest_frame, 0))
        print(test)
        test += 1
        return base64.b64encode(img_encoded).decode()
    

    def process_results(self, result):
        print(result)
        route = {"left": "L", "right": "R", "scan": "S"}.get(result)

        try:
            response = requests.get(config["servo_ip"] + f"/{route}", timeout=1)
            print(response.text)
        except requests.exceptions.RequestException as e:
            print(e)

        time.sleep(0.1)
        return True


def main() -> None:
    global is_running
    webview.create_window("Smart Desk Lamp", "index.html", js_api=API())
    webview.start(webview_thread, debug=True)
    is_running = False


def webview_thread() -> None:
    global latest_frame

    while is_running:
        ret, frame = cap.read()

        if ret:
            latest_frame = frame


if __name__ == "__main__":
    main()