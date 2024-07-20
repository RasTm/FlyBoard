Bu proje uçuş kontrol kartımız için gerekli yazılımım içermektedir. Proje tamamlandıkça bu bölüm kullanım klavuzu olarak değiştirilecektir.

Projeyi Atollic ile derleyebilmek için aşağıdaki işlemler yapılmalıdır.
1- C++ sürümü Properties (Alt+Enter) / C,C++ Build / Settings / Tool Settings / C++ Compiler / General başlığı altından C++ 11 olarak ayarlanmalıdır.
2- Properties (Alt+Enter) / C,C++ General / Paths and Symbols başlığına gidilip Includes, Library Paths, Source Location sekmelerine Libs klasörü eklenmelidir.
3- Float değerleri LCD ekran üzerine yazdırmak için snprint fonksiyonu modifiye edilmelidir. Bunun için Properties (Alt+Enter) / C,C++ Build / Settings / Tool Settings / C++ Linker başlığı içerisinde Command alt başlığına "-u_printf_float" ifadesi yazılmalıdır.
