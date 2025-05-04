import webview
import cv2
import base64
import requests
import json
import time
import flask
import threading


config = json.load(open("config.json"))
cap = cv2.VideoCapture(config["camera_ip"] + ":4747/video")
latest_frame = None
is_running = True
test = 0
save_image_cooldown = 0
update_frame_cooldown = 0
flask_app = flask.Flask(__name__)
alarm_enabled = True


class API:
    def get_frame(self):
        global test, update_frame_cooldown
        _, img_encoded = cv2.imencode(".jpg", cv2.flip(latest_frame, 0))
        print(test)
        test += 1
        return base64.b64encode(img_encoded).decode()
    

    def process_results(self, results):
        global save_image_cooldown
        print(results)
        route = {"found": "F", "left": "L", "right": "R", "scan": "S"}.get(results)

        if alarm_enabled:
            tone = "T"
        else:
            tone = "N"

        try:
            if route in ["F", "L", "R"] and time.time() > save_image_cooldown:
                save_image_cooldown = time.time() + 30
                cv2.imwrite(f"saved/{time.strftime('%Y-%m-%d_%H-%M-%S')}.jpg", latest_frame)

            response = requests.get(config["servo_ip"] + f"/{route}{tone}", timeout=1)
            print(response.text)
        except requests.exceptions.RequestException as e:
            print(e)

        time.sleep(0.1)
        return True
    

@flask_app.route('/')
def index():
    # output app.html
    return flask.render_template('app.html')


@flask_app.route('/set_alarm', methods=['POST'])
def set_alarm():
    global alarm_enabled
    data = flask.request.get_json()
    alarm_enabled = data.get('enabled', False)
    print(f"Alarm enabled: {alarm_enabled}")
    return flask.jsonify({'success': True})


@flask_app.route('/video_feed')
def video_feed():
    return flask.Response(generate_mjpeg(), mimetype='multipart/x-mixed-replace; boundary=frame')


def generate_mjpeg():
    global latest_frame

    while True:
        if latest_frame is not None:
            ret, jpeg = cv2.imencode('.jpg', latest_frame)

            if ret:
                frame = jpeg.tobytes()

                yield (
                    b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n'
                )

        time.sleep(0.1)


def run_flask():
    flask_app.run(host='192.168.1.102', port=5000)


def main() -> None:
    global is_running
    webview.create_window("Smart Desk Lamp", "index.html", js_api=API())
    threading.Thread(target=run_flask, daemon=True).start()
    webview.start(webview_thread, debug=True)
    is_running = False


def webview_thread() -> None:
    global latest_frame

    while is_running:
        try:
            ret, frame = cap.read()

            if ret:
                latest_frame = frame
        except Exception as e:
            print(e)
            time.sleep(1)


if __name__ == "__main__":
    main()