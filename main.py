import cv2
import flask
import config


app = flask.Flask(__name__)
enable_alarm = False
enable_tracking = False


@app.route('/')
def index():
    return flask.render_template('index.html', servo_ip=config.servo_ip)


@app.route('/video_feed')
def video_feed():
    return flask.Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


def generate_frames():
    camera = cv2.VideoCapture(f"http://{config.camera_ip}:4747/video")

    while True:
        success, frame = camera.read()

        if not success:
            continue

        _, buffer = cv2.imencode('.jpg', frame)
        frame = buffer.tobytes()

        yield (
            b'--frame\r\n'
            b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n'
        )


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)