# 🔒 Akıllı Kasa Projesi (Smart Safe Box)

Bu proje, ESP32 tabanlı, şifreli ve LCD ekranlı bir güvenlik sistemi prototipidir.

## 🌟 Özellikler
* **Güvenlik:** 4 haneli şifre koruması.
* **Kullanıcı Arayüzü:** I2C LCD Ekran ve Sesli/Işıklı geri bildirim.
* **Donanım:** Özel tasarım PCB Anakart.
* **Yazılım:** C++ Class yapısı, Linked List ile dinamik şifre yönetimi(V1/V2) ve State Machine mimarisi.
* **Ayarlar:** Ses ve Işık açma/kapama özelliği.

## 🛠️ Donanım Bileşenleri
* ESP32 DevKit V1
* MG90S Servo Motor
* 16x2 I2C LCD Ekran
* Aktif Buzzer
* RGB LED

## 📂 Dosya Yapısı
* **Hardware:** PCB üretim dosyaları (Gerber) ve şematikler.
* **Firmware:** PlatformIO (VS Code) kaynak kodları.
