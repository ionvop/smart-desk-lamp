import webview
import cv2
import base64
import requests
import json


config = json.load(open("config.json"))
cap = cv2.VideoCapture(config["camera_ip"] + ":81/stream")
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
latest_frame = None
is_running = True


class API:
    def get_frame(self):
        _, img_encoded = cv2.imencode(".jpg", latest_frame)
        return base64.b64encode(img_encoded).decode()
    

    def process_results(self, result):
        print(result)
        route = {"left": "L", "right": "R", "up": "U", "down": "D"}.get(result)

        try:
            response = requests.get(config["servo_ip"] + f"/{route}", timeout=1)
            print(response.text)
        except requests.exceptions.RequestException as e:
            print(e)


def main() -> None:
    global is_running
    webview.create_window("Smart Desk Lamp", "index.html", js_api=API())
    webview.start(webview_thread)
    is_running = False


def webview_thread() -> None:
    global latest_frame

    while is_running:
        ret, frame = cap.read()

        if ret:
            latest_frame = frame


if __name__ == "__main__":
    main()