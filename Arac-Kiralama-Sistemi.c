/*
 
Ad Soyad: Zeren Kızılsu
Numara: 25118080067
Proje: Araç Kiralama Sistemi

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;               // Arac kimlik numarasi
    char marka[50];       // Arac markasi
    char model[50];       // Arac modeli
    int yil;
    int durum;            // 1: Müsait demektir, 0: Kiradadir demektir.
} Arac;

typedef struct {
    long long id;         // TC kimlik no
    char ad[50];          // Uye adi
    char telefon[15];     // Uyenin telefon numarasi
} Musteri;

typedef struct {
    int aracID;           // Kiralanan aracin ID'si
    long long musteriID;  // Kiralayan musterinin ID'si
    char kiralamaTarihi[20];
    char teslimTarihi[20];
} Kiralama;


int aracSayisiniBul();
void aracEkle();
void araclariListele();
void araciSil();
void menuyuGoster();
void aracKirala();
void kiralamaListesi();
void musteriEkle();
void musteriListele();


int main() {
    int secim;

    while (1) {
        menuyuGoster();
        printf("\nSeciminizi giriniz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1: aracEkle(); 
              break;
            case 2: araclariListele(); 
              break; // Burada malloc çalışacak
            case 3: araciSil(); 
              break;
            case 4: musteriEkle(); 
              break;
            case 5: musteriListele(); 
              break;
            case 6: aracKirala(); 
              break;
            case 7: kiralamaListesi(); 
              break;
            case 0: 
                printf("Program sonlandiriliyor...\n");
                return 0;
            default: 
                printf("Hatali secim! Tekrar deneyin.\n");
        }
    }
    return 0;
}


int aracSayisiniBul() {
    FILE *fp = fopen("araclar.txt", "r");
    if (fp == NULL) return 0;

    int sayac = 0;
    char ch;
    while (!feof(fp)) {
        ch = fgetc(fp);
        if (ch == '\n') {
            sayac++;
        }
    }
    fclose(fp);
    return sayac;
}

void aracEkle() {
    FILE *fp = fopen("araclar.txt", "a"); 
    if (fp == NULL) {
        printf("Dosya acilamadi!\n");
        return;
    }

    Arac yeniArac;
    printf("\n--- ARAC EKLEME ---\n");
    printf("Arac ID: "); scanf("%d", &yeniArac.id);
    printf("Marka: "); scanf("%s", yeniArac.marka);
    printf("Model: "); scanf("%s", yeniArac.model);
    printf("Yil: "); scanf("%d", &yeniArac.yil);
    yeniArac.durum = 1; // 1: Müsait demektir

    // Dosyaya yazma sırası ID, marka, model, yıl, durum şeklindedir.
    fprintf(fp, "%d %s %s %d %d\n", yeniArac.id, yeniArac.marka, yeniArac.model, yeniArac.yil, yeniArac.durum);
    
    fclose(fp);
    printf("Arac basariyla eklendi.\n");
}

void araclariListele() {
    int adet = aracSayisiniBul();
    if (adet == 0) {
        printf("Listelenecek arac yok.\n");
        return;
    }

    // Araç sayısı kadar hafızada dinamik yer ayıracağız.
    Arac *dizi = (Arac *)malloc(adet * sizeof(Arac)); 

    FILE *fp = fopen("araclar.txt", "r");
    
    printf("\n--- ARAC LISTESI ---\n");
    printf("ID\tMarka\tModel\tYil\tDurum\n");
    printf("---------------------------------------\n");

    int i = 0;
    // Dosyadan okuyup diziye atıyoruz
    while (fscanf(fp, "%d %s %s %d %d", &dizi[i].id, dizi[i].marka, dizi[i].model, &dizi[i].yil, &dizi[i].durum) != EOF) {
        printf("%d\t%s\t%s\t%d\t%s\n", 
            dizi[i].id, 
            dizi[i].marka, 
            dizi[i].model, 
            dizi[i].yil, 
            dizi[i].durum == 1 ? "Musait" : "Kirada");
        i++;
    }

    fclose(fp);
    free(dizi); // İşimiz bitince yerlerin dolmaması için belleği temizliyoruz.
}

void araciSil() {
    int silinecekID;
    printf("Silinecek Arac ID: ");
    scanf("%d", &silinecekID);

    FILE *fp = fopen("araclar.txt", "r");
    FILE *tempFp = fopen("temp.txt", "w"); 
    if (fp == NULL) return;

    Arac temp;
    int bulundu = 0;
    // Dosyayı baştan sona okuyoruz
    while (fscanf(fp, "%d %s %s %d %d", &temp.id, temp.marka, temp.model, &temp.yil, &temp.durum) != EOF) {
        if (temp.id != silinecekID) {
            // Silinmeyecek olanları geçici dosyaya kopyala
            fprintf(tempFp, "%d %s %s %d %d\n", temp.id, temp.marka, temp.model, temp.yil, temp.durum);
        } else {
            bulundu = 1;
        }
    }

    fclose(fp);
    fclose(tempFp);

    if (bulundu) {
        remove("araclar.txt");       //Eski dosyayı silmek için
        rename("temp.txt", "araclar.txt"); //Yeni dosyayı asıl dosya yapmak için
        printf("Arac silindi.\n");
    } else {
        remove("temp.txt");
        printf("Arac bulunamadi.\n");
    }
}




void menuyuGoster() {
    printf("\n=====ARAC KIRALAMA SISTEMI=====\n");
    printf("1. Arac Ekle \n");
    printf("2. Arac Listele \n");
    printf("3. Arac Sil \n");
    printf("--------------------------------\n");
    printf("4. Musteri Ekle \n");
    printf("5. Musteri Listele \n");
    printf("-------------------------------- \n");
    printf("6. Arac Kirala \n");
    printf("7. Kiralama Listesi \n");
    printf("0. Cikis \n");
}

void aracKirala() {
    Kiralama k;
    printf("\n------ARAC KIRALA------ \n");
    printf("Kiralanacak Arac ID: "); scanf("%d", &k.aracID);
    printf("Musteri TC: "); scanf("%lld", &k.musteriID);
    printf("Kiralama Tarihi (GG.AA.YYYY): "); scanf("%s", k.kiralamaTarihi);
    strcpy(k.teslimTarihi, "-"); 

    FILE *fp = fopen("kiralama.txt", "a");
    fprintf(fp, "%d %lld %s %s\n", k.aracID, k.musteriID, k.kiralamaTarihi, k.teslimTarihi);
    fclose(fp);
    
    printf("Kiralama islemi kaydedildi.\n");
}

void kiralamaListesi() {
    FILE *fp = fopen("kiralama.txt", "r");
    if (fp == NULL) {
        printf("Kiralama kaydi yok.\n");
        return;
    }
    
    Kiralama k;
    printf("\n--- KIRALAMA GECMISI ---\n");
    while (fscanf(fp, "%d %lld %s %s", &k.aracID, &k.musteriID, k.kiralamaTarihi, k.teslimTarihi) != EOF) {
        printf("Arac ID: %d | Musteri TC: %lld | Tarih: %s\n", k.aracID, k.musteriID, k.kiralamaTarihi);
    }
    fclose(fp);
}



void musteriEkle() {
    FILE *fp = fopen("musteriler.txt", "a");
    if (fp == NULL) return;

    Musteri m;
    printf("\n--- MUSTERI EKLE ---\n");
    printf("TC Kimlik (11 hane): "); scanf("%lld", &m.id); 
    printf("Ad Soyad (Bosluksuz): "); scanf("%s", m.ad);
    printf("Telefon: "); scanf("%s", m.telefon);

    fprintf(fp, "%lld %s %s\n", m.id, m.ad, m.telefon);
    fclose(fp);
    printf("Musteri eklendi.\n");
}

void musteriListele() {
    FILE *fp = fopen("musteriler.txt", "r");
    if (fp == NULL) {
        printf("Kayitli musteri yok.\n");
        return;
    }

    Musteri m;
    printf("\n--- MUSTERI LISTESI ---\n");
    printf("TC\t\tIsim\t\tTelefon\n");
    while (fscanf(fp, "%lld %s %s", &m.id, m.ad, m.telefon) != EOF) {
        printf("%lld\t%s\t%s\n", m.id, m.ad, m.telefon);
    }
    fclose(fp);
}