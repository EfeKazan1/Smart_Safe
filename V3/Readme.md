# 🔐 ESP32 Smart Safe System (v3.0)

**Gelişmiş Hafıza Yönetimi ve Nesne Yönelimli Mimari ile Tasarlanmış Akıllı Kasa Sistemi**

Bu proje, standart bir Arduino projesinin ötesine geçerek; **Finite State Machine (FSM)**, **Non-Blocking I/O** ve **Persistent Memory** prensipleri kullanılarak geliştirilmiş profesyonel bir gömülü sistem uygulamasıdır.

## 🚀 Proje Özellikleri (v3.0 Yenilikleri)

Bu sürümde (v3), spagetti kod yapısı terk edilerek tamamen modüler ve ölçeklenebilir bir mühendislik mimarisine geçilmiştir.

### 🛠 Yazılım Mimarisi
- **OOP (Nesne Yönelimli Programlama):** Donanım (Pinler) ve Mantık (Şifre) birbirinden soyutlanarak `class` yapısına alındı.
- **Finite State Machine (FSM):** Kasa durumu `switch-case` yapısı ile yönetilerek (GIRIS -> KONTROL -> ACIK -> KILITLI) kararlı bir akış sağlandı.
- **Non-Blocking Multitasking:** `delay()` fonksiyonu yerine `millis()` tabanlı zamanlayıcılar kullanıldı. Bu sayede kasa açıkken veya şifre girilirken arka planda butonlar (LED/SES) algılanmaya devam eder.
- **Encapsulation & Security:** Şifre verileri `private` değişkenlerde tutularak dış erişime kapatıldı.

### 💾 Hafıza Yönetimi (Memory Management)
- **Preferences API (Kalıcı Hafıza):** Eski tip EEPROM yerine, ESP32'nin modern `Preferences.h` kütüphanesi kullanıldı.
  - **Şifre:** Cihaz kapansa bile hatırlanır.
  - **Kullanıcı Ayarları:** Ses (Mute) ve LED durumu elektrik kesintisinde bile korunur.
- **RAM Optimizasyonu:** `Linked List` gibi dinamik bellek yapıları yerine sabit boyutlu diziler (Arrays) kullanılarak **Heap Fragmentation** riski sıfıra indirildi.

## ⚙️ Donanım Gereksinimleri

| Bileşen | Açıklama |
| :--- | :--- |
| **Mikrodenetleyici** | ESP32 Dev Kit V1 |
| **Aktüatör** | SG90 Servo Motor |
| **Ekran** | 16x2 LCD (I2C Modüllü) |
| **Giriş** | 4x Push Button (Arttır, Onayla, Led, Ses) |
| **Geri Bildirim** | RGB LED (Ortak Katot/Anot) ve Buzzer |

## 🔌 Pin Bağlantı Şeması (Pinout)

Projede kullanılan `Pins` sınıfı yapılandırması şöyledir:

| Bileşen | ESP32 GPIO Pin |
| :--- | :--- |
| **RGB Kırmızı** | 27 |
| **RGB Yeşil** | 26 |
| **RGB Mavi** | 25 |
| **Buton (Rakam Arttır)** | 23 |
| **Buton (Onayla)** | 18 |
| **Servo Motor** | 32 |
| **Buzzer** | 33 |
| **Buton (Ses Aç/Kapa)** | 19 |
| **Buton (Led Aç/Kapa)** | 5 |
| **I2C SDA** | 21 |
| **I2C SCL** | 22 |

## 📂
