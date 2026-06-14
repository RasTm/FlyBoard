Bu proje uçuş kontrol kartımız için gerekli yazılımım içermektedir. Proje tamamlandıkça bu bölüm kullanım klavuzu olarak değiştirilecektir.

Projeyi Atollic ile derleyebilmek için aşağıdaki işlemler yapılmalıdır.

1- Proje açılırken executable değil makefile olarak açılmalıdır.

2- C++ sürümü Properties (Alt+Enter) / C,C++ Build / Settings / Tool Settings / C++ Compiler / General başlığı altından C++ 11 olarak ayarlanmalıdır.

3- Properties (Alt+Enter) / C,C++ General / Paths and Symbols başlığına gidilip Includes, Library Paths, Source Location sekmelerine Libs klasörü eklenmelidir.

4- Properties (Alt+Enter) / C,C++ General / Paths and Symbols başlığına gidilip Includes ve Symbols sekmelerinden STD_Lib ile alakalı her şey kaldırılmalıdır. 

5- Libraries klasöründeki STD_periph klasörü kaldırılmalıdır. 

6- Libraries/Cmsis klasörü içerisine DSP_Lib ve Include klasörleri atılmalıdır eğer uyarı verirse orijinal dosyaların üzerine yazılmalıdır.

7- stm32f4xx_conf.h dosyasında eklenmiş tüm Stdlib kütüphaneleri kaldırılmalıdır.

8- Float değerleri LCD ekran üzerine yazdırmak için snprint fonksiyonu modifiye edilmelidir. Bunun için Properties (Alt+Enter) / C,C++ Build / Settings / Tool Settings / C++ Linker başlığı içerisinde Command alt başlığına "-u_printf_float" ifadesi yazılmalıdır.

9- Debug ekranında başlatmadan önce main'de durmasını sağlayan breakpoint kaldırılmalı yoksa C++ ile geliştirme yaparken program kırılabilir. Kaldırmak için Configure Debug / Startup Scripts gidilmeli. Buranın en altından 
"# Set a breakpoint at main(). tbreak main" Silinmeli.
