# 🛡️ ESP32 IoT Smart Safe (v4.0) - Dual Core & FreeRTOS Implementation

**Gerçek Zamanlı İşletim Sistemi (RTOS) Mimarisi ile Geliştirilmiş, Çift Çekirdekli Uzaktan Erişim Sistemi**

Bu proje, önceki versiyonların üzerine inşa edilerek; sistemin **Bloklanamaz (Non-Blocking)** yapısını korumak ve ağ işlemlerini ana döngüden izole etmek amacıyla **ESP32 Dual Core** mimarisi kullanılarak geliştirilmiştir.

## 🚀 Projenin Mühendislik Odak Noktası

Gömülü sistemlerde yaygın bir sorun olan "Ağ bağlantısı sırasında işlemcinin donması (Blocking I/O)" problemi, bu projede **Görev Paralelleştirme (Task Parallelism)** yöntemiyle çözülmüştür.

### 🧠 Çift Çekirdek Mimarisi (Dual Core Architecture)

Sistem yükü, ESP32'nin iki çekirdeği arasında asimetrik olarak dağıtılmıştır:

| Çekirdek (Core) | Görev (Task) | Sorumluluk |
| :--- | :--- | :--- |
| **Core 1 (App Core)** | `TaskKasa` | **Gerçek Zamanlı İşlemler:** Tuş takımı okuma, LCD güncelleme, Servo motor kontrolü ve Şifre doğrulama algoritmaları. (Latency-Critical) |
| **Core 0 (Pro Core)** | `TaskWiFi` | **Arka Plan İşlemleri:** Wi-Fi bağlantısı yönetimi, TCP/IP Web Sunucusu ve HTTP isteklerinin işlenmesi. (Network-Heavy) |

Bu yapı sayesinde, Wi-Fi bağlantısı kopsa veya web sunucusu yoğun olsa bile, kasanın fiziksel arayüzü (tuşlar ve ekran) **milisaniye bile takılmadan** çalışmaya devam eder.

## 🛠 Kullanılan Teknolojiler ve Yöntemler

- **FreeRTOS:** `xTaskCreatePinnedToCore` ile görev yönetimi ve zamanlama.
- **Embedded Web Server:** HTML/CSS arayüzü `PROGMEM` üzerinde saklanarak, harici SD kart gerektirmeyen hafif bir Web Arayüzü oluşturuldu.
- **RESTful-like API:** Web arayüzü ile cihaz arasındaki haberleşme HTTP GET istekleri üzerinden (örn: `/kontrol?sifre=1923`) sağlanır.
- **Persistent Memory (NVM):** `Preferences.h` kütüphanesi ile şifre ve ayarlar Flash hafızada güvenli bir şekilde saklanır.
- **OOP (Nesne Yönelimli Programlama):** Donanım soyutlama katmanları (HAL) `Pins` ve `Password` sınıfları ile modüler hale getirilmiştir.

## 📱 Web Arayüzü Özellikleri

Cihaz, yerel ağ üzerinde (Localhost) bir Web Sunucusu başlatır. Kullanıcı IP adresine bağlandığında:
- **Responsive Arayüz:** Mobil uyumlu sanal tuş takımı.
- **Uzaktan Şifre Yönetimi:**
  - Eğer kasa **kurulum modundaysa (GIRIS)**: Web üzerinden gönderilen şifre **YENİ ŞİFRE** olarak kaydedilir.
  - Eğer kasa **normal moddaysa**: Gönderilen şifre doğrulanır ve kapı açılır.

## ⚙️ Donanım Şeması

| Bileşen | ESP32 GPIO |
| :--- | :--- |
| **Servo Motor** | 32 |
| **I2C LCD (SDA/SCL)** | 21, 22 |
| **Butonlar (Arttır, Onayla, Led, Ses)** | 23, 18, 5, 19 |
| **RGB Led (R, G, B)** | 27, 26, 25 |
| **Buzzer** | 33 |

## 📂 Dosya Yapısı

- `main.cpp`: FreeRTOS görevlerinin dağıtımı ve Web Sunucusu yönetimi.
- `efe_lib.h / .cpp`: Şifreleme algoritmaları, hafıza yönetimi ve donanım kontrol sınıfları.

## 🚀 Kurulum

1. `main.cpp` içerisindeki `WIFI ADIN` ve `WIFI SIFREN` alanlarını düzenleyin.
2. PlatformIO veya Arduino IDE kullanarak ESP32 kartına yükleyin.
3. Seri Monitörden (115200 baud) cihazın aldığı IP adresini öğrenin.
4. Tarayıcıdan IP adresine giderek sistemi kontrol edin.

---
*Geliştirici: Efe Kazan*
*Bilgisayar Mühendisliği Öğrencisi*
