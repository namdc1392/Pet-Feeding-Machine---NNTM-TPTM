#include <HX711.h>
#include <Servo.h>

// Loadcell
#define DT 3
#define SCK 2

// Servo
#define SERVO_PIN 10
#define SERVO_OPEN_ANGLE 90
#define SERVO_CLOSE_ANGLE 0

// HX711 và Servo
HX711 scale;
Servo feederServo;

// Ngưỡng điều kiện
#define SCALE_FACTOR 420.0  // Hiệu chuẩn thực tế
#define LOWER_THRESHOLD 30.0  // < 30g → mở
#define UPPER_THRESHOLD 60.0  // > 60g → đóng

// Trạng thái servo hiện tại
bool isServoOpen = false;

void setup() {
  Serial.begin(9600);

  scale.begin(DT, SCK);
  scale.set_scale(SCALE_FACTOR);
  scale.tare();

  feederServo.attach(SERVO_PIN);
  feederServo.write(SERVO_CLOSE_ANGLE);  // ban đầu đóng
  isServoOpen = false;

  Serial.println("🚀 Hệ thống bắt đầu...");
}

void loop() {
  float weight = scale.get_units(10);  // lấy trung bình 10 lần đọc

  Serial.print("Khối lượng hiện tại (g): ");
  Serial.println(weight, 1);

  // Nếu nhỏ hơn 30g → mở nếu chưa mở
  if (weight < LOWER_THRESHOLD && !isServoOpen) {
    Serial.println("⚠️ Trọng lượng thấp! Mở servo.");
    feederServo.write(SERVO_OPEN_ANGLE);
    isServoOpen = true;

  // Nếu lớn hơn 60g → đóng nếu chưa đóng
  } else if (weight > UPPER_THRESHOLD && isServoOpen) {
    Serial.println("✅ Trọng lượng đủ. Đóng servo.");
    feederServo.write(SERVO_CLOSE_ANGLE);
    isServoOpen = false;
  }

  delay(300);  // chờ trước khi đọc lại
}
