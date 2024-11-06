#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <raylib.h>
#include <time.h>
int ekran_genislik = 1500;
int ekran_uzunluk = 1024;
int izgara_boyut = 1024;
int kare_boyut = 40;
typedef struct {
    Vector2 yer;
    Texture2D resim;
    float boyut;
    int can;
    int max_can;
    char isim[20];
     int birim_sayisi;
} Birim;
typedef struct {
float saldiri_total;
float savunma_total;
float saglik_total;
float net_hasar;
}savas_hesapla;
typedef struct {
    float deger_float;
    char deger[20];
    char aciklama[200];
    char etkilenen_birim[50];
} level;
typedef struct {
int piyade_sayi;
int suvari_sayi;
int okcu_sayi;
int kusatmamak_sayi;
int saldiri_seviye;
int savunma_seviye;
int kusatma_seviye;
int elit_seviye;
char kahraman[100];
char kahraman1[100];
char canavar[100];
char canavar1[100];

}insan_savas;
typedef struct {
int ork_dovusculeri_sayi;
int mizrakcilar_sayi;
int varg_binicileri_sayi;
int troller_sayi;
int saldiri_seviye;
int savunma_seviye;
int kusatma_seviye;
int elit_seviye;
char kahraman[100];
char kahraman1[100];
char canavar[100];
char canavar1[100];
}ork_savas;
typedef struct {
    level level_1;
    level level_2;
    level level_3;
}ustalik;
typedef struct {
    char etki_degeri[20];
    float etki_degeri_float;
    char etki_turu[20];
    char etkiledigi_birim[100];
    char aciklama[200];
} orc_yaratik;
typedef struct {
    char etki_degeri[20];
    float etki_degeri_float;
    char etki_turu[20];
    char etkiledigi_birim[50];
    char aciklama[200];
} insan_yaratik;
typedef struct {
    float saldiri;
    float savunma;
    float saglik;
    int kritik_sans;
    float saldiri_kritik;
}asker;
typedef struct {
asker piyadeler;
asker okcular;
asker suvariler;
asker kusatma_makineleri;
}insan_imp;
typedef struct {
asker ork_dovusculeri;
asker mizrakcilar;
asker varg_binicileri;
asker troller;
}ork_legi;
typedef struct {
    char bonus_turu[20];
    char etkiledigi_birim[50];
    char bonus_degeri[20];
    float bonus_degeri_float;
    char aciklama[200];
} ork_hero;
void canbari_olustur(Vector2 yer, float can, float max_can) {
    float bar_genislik = 40 ;
    float bar_yukseklik = 8 ;
    float can_yuzdesi = can / max_can;
    float dolucanalan = bar_genislik * can_yuzdesi;

    DrawRectangle(yer.x, yer.y - 10 , bar_genislik, bar_yukseklik, RED);
    DrawRectangle(yer.x, yer.y - 10 , dolucanalan, bar_yukseklik, GREEN);
    DrawRectangleLines(yer.x, yer.y - 10 , bar_genislik, bar_yukseklik, BLACK);
}
typedef struct {
    char bonus_turu[20];
    char etkiledigi_birim[50];
    char bonus_degeri[20];
    float bonus_degeri_float;
    char aciklama[200];
} insan_hero;
char* dosya_oku(const char* dosya) {
    FILE *file = fopen(dosya, "r");
    if (file == NULL) {
        printf("Dosya acilamadi.");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    int dosya_boyutu = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *dosya_ic = (char*)malloc(sizeof(char) * (dosya_boyutu + 1));
    fread(dosya_ic, 1, dosya_boyutu, file);
    dosya_ic[dosya_boyutu] = '\0';
    fclose(file);
    return dosya_ic;
}
void aranandegerbul(const char *json, const char *ad, const char *ad1, char *deger) {
    char *ad_bul = strstr(json, ad);
    if (ad_bul) {
        char *ad_bul1 = strstr(ad_bul, ad1);
        if (ad_bul1) {
            char *iki_nokta = strchr(ad_bul1, ':');
            if (iki_nokta) {
                iki_nokta++;
                while (*iki_nokta == ' ' || *iki_nokta == '\"')
                iki_nokta++;
                int i = 0;
                while (iki_nokta[i] && iki_nokta[i] != ',' && iki_nokta[i] != '}' && iki_nokta[i] != '\"') {
                    deger[i] = iki_nokta[i];
                    i++;
                }
                deger[i] = '\0';
            }
        }
    }
}
void aranandegerbul_research(const char *json, const char *ad, const char *ad1, const char *ad2, char *deger) {
    char *ad_bul = strstr(json, ad);
    if (ad_bul) {
        char *ad_bul1= strstr(ad_bul, ad1);
        if (ad_bul1) {
            char *ad_bul2 = strstr(ad_bul, ad2);
            if (ad_bul2) {
                char *iki_nokta = strchr(ad_bul2, ':');
                if (iki_nokta) {
                    iki_nokta++;
                    while (*iki_nokta == '\"'||*iki_nokta == ' ')
                    iki_nokta++;
                    int i = 0;
                    while (iki_nokta[i] && iki_nokta[i] != ',' && iki_nokta[i] != '}' && iki_nokta[i] != '\"') {
                        deger[i] = iki_nokta[i];
                        i++;
                    }
                    deger[i] = '\0';
                }
            }
        }
    }
}
void aranandegerbul_ustalik(const char *json, const char *ad, const char *ad1, char *deger) {
    char *ad_bul = strstr(json, ad);
    if (ad_bul) {
        char *arastirma_bul = strstr(ad_bul, "\"arastirma_seviyesi\"");
        if (arastirma_bul) {
            char *son_bul = strchr(arastirma_bul, '}');
            if (son_bul) {
                char *ad_bul1 = strstr(arastirma_bul, ad1);
                if (ad_bul1 && ad_bul1 < son_bul) {
                    char *iki_nokta = strchr(ad_bul1, ':');
                    if (iki_nokta) {
                        iki_nokta++;
                        while (*iki_nokta == '\"' || *iki_nokta == ' ')
                            iki_nokta++;
                        int i = 0;
                        while (iki_nokta[i] && iki_nokta[i] != ',' && iki_nokta[i] != '}' && iki_nokta[i] != '\"') {
                            deger[i] = iki_nokta[i];
                            i++;
                        }
                        deger[i] = '\0';
                    }
                }
            }
        }
    }
}
void aranandegerbul_senaryo(const char *json, const char *ad, const char *ad1, char *deger1, char *deger2){
    char *ad_bul = strstr(json, ad);
    if (ad_bul)
    {
        char *ad_bul1 = strstr(ad_bul, ad1);
        if (ad_bul1)
        {
            char *iki_nokta = strchr(ad_bul1, ':');
            if (iki_nokta)
            {
                iki_nokta++;
                while (*iki_nokta == ' ' || *iki_nokta == '\"' || *iki_nokta == '[')
                    iki_nokta++;
                int i = 0;
                while (iki_nokta[i] && iki_nokta[i] != ',' && iki_nokta[i] != ']' && iki_nokta[i] != '\"')
                {
                    deger1[i] = iki_nokta[i];
                    i++;
                }
                deger1[i] = '\0';
                char *virgul = strchr(iki_nokta, ',');
                if (virgul)
                {
                    virgul++;
                    while (*virgul == ' ' || *virgul == '\"')
                        virgul++;

                    i = 0;
                    while (virgul[i] && virgul[i] != ']' && virgul[i] != '\"')
                    {
                        deger2[i] = virgul[i];
                        i++;
                    }
                    deger2[i] = '\0';
                }
                else
                {
                    deger2[0] = '\0';
                }
            }
        }
    }
    else
    {
        deger1[0] = '\0';
        deger2[0] = '\0';
    }
}
void bilgi_yukle_ustalik(const char* json, ustalik* savunma, ustalik* saldiri, ustalik* elit_egitim, ustalik* kusatma) {
    aranandegerbul_research(json, "savunma_ustaligi","seviye_1","deger", savunma->level_1.deger);
    aranandegerbul_research(json,"savunma_ustaligi","seviye_1","aciklama",savunma->level_1.aciklama);
    savunma->level_1.deger_float = atof(savunma->level_1.deger);
    aranandegerbul_research(json, "savunma_ustaligi","seviye_2","deger", savunma->level_2.deger);
     aranandegerbul_research(json,"savunma_ustaligi","seviye_2","aciklama",savunma->level_2.aciklama);
    savunma->level_2.deger_float = atof(savunma->level_2.deger);
    aranandegerbul_research(json, "savunma_ustaligi","seviye_3","deger", savunma->level_3.deger);
    aranandegerbul_research(json, "savunma_ustaligi","seviye_3","aciklama", savunma->level_3.aciklama);
    savunma->level_3.deger_float = atof(savunma->level_3.deger);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_1","deger", saldiri->level_1.deger);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_1","aciklama", saldiri->level_1.aciklama);
    saldiri->level_1.deger_float=atof(saldiri->level_1.deger);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_2","aciklama", saldiri->level_2.aciklama);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_2","deger", saldiri->level_2.deger);
    saldiri->level_2.deger_float=atof(saldiri->level_2.deger);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_3","aciklama", saldiri->level_3.aciklama);
    aranandegerbul_research(json, "saldiri_gelistirmesi","seviye_3","deger", saldiri->level_3.deger);
    saldiri->level_3.deger_float=atof(saldiri->level_3.deger);
    aranandegerbul_research(json, "elit_egitim","seviye_1","aciklama", elit_egitim->level_1.aciklama);
    aranandegerbul_research(json, "elit_egitim","seviye_1","deger", elit_egitim->level_1.deger);
    elit_egitim->level_1.deger_float=atof(elit_egitim->level_1.deger);
    aranandegerbul_research(json, "elit_egitim","seviye_2","aciklama", elit_egitim->level_2.aciklama);
    aranandegerbul_research(json, "elit_egitim","seviye_2","deger", elit_egitim->level_2.deger);
    elit_egitim->level_2.deger_float=atof(elit_egitim->level_2.deger);
    aranandegerbul_research(json, "elit_egitim","seviye_3","aciklama", elit_egitim->level_3.aciklama);
    aranandegerbul_research(json, "elit_egitim","seviye_3","deger", elit_egitim->level_3.deger);
    elit_egitim->level_3.deger_float=atof(elit_egitim->level_3.deger);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_1","aciklama", kusatma->level_1.aciklama);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_1","deger", kusatma->level_1.deger);
    kusatma->level_1.deger_float=atof(kusatma->level_1.deger);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_2","aciklama", kusatma->level_2.aciklama);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_2","deger", kusatma->level_2.deger);
    kusatma->level_2.deger_float=atof(kusatma->level_2.deger);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_3","aciklama", kusatma->level_3.aciklama);
    aranandegerbul_research(json, "kusatma_ustaligi","seviye_3","deger", kusatma->level_3.deger);
    kusatma->level_3.deger_float=atof(kusatma->level_3.deger);
}
void hero_yukle(const char* json, const char* orkhero_ad,const char* insanhero_ad,ork_hero* ork,insan_hero* insan) {
    aranandegerbul(json, orkhero_ad, "bonus_turu", ork->bonus_turu);
    aranandegerbul(json, orkhero_ad, "bonus_degeri", ork->bonus_degeri);
    aranandegerbul(json, orkhero_ad, "aciklama", ork->aciklama);
    aranandegerbul(json, insanhero_ad, "bonus_turu", insan->bonus_turu);
    aranandegerbul(json, insanhero_ad, "bonus_degeri", insan->bonus_degeri);
    aranandegerbul(json, insanhero_ad, "aciklama", insan->aciklama);
    insan->bonus_degeri_float = atof(insan->bonus_degeri);
    ork->bonus_degeri_float = atof(ork->bonus_degeri);
}
void asker_yukle(const char *json, const char *asker_ad, asker *asker) {
    char deger[20];
    aranandegerbul(json, asker_ad, "saldiri", deger);
    asker->saldiri = atof(deger);
    aranandegerbul(json, asker_ad, "savunma", deger);
    asker->savunma = atof(deger);
    aranandegerbul(json, asker_ad, "saglik", deger);
    asker->saglik = atof(deger);
    aranandegerbul(json, asker_ad, "kritik_sans", deger);
    asker->kritik_sans = atoi(deger);
}
void unit_yukle(const char *json, insan_imp *insan, ork_legi *ork) {
    asker_yukle(json, "piyadeler", &insan->piyadeler);
    asker_yukle(json, "okcular", &insan->okcular);
    asker_yukle(json, "suvariler", &insan->suvariler);
    asker_yukle(json, "kusatma_makineleri", &insan->kusatma_makineleri);
    asker_yukle(json, "ork_dovusculeri", &ork->ork_dovusculeri);
    asker_yukle(json, "mizrakcilar", &ork->mizrakcilar);
    asker_yukle(json, "varg_binicileri", &ork->varg_binicileri);
    asker_yukle(json, "troller", &ork->troller);
}
void yaratik_yukle(const char *json,const char*orkyaratik_ad,const char* insanyaratik_ad,orc_yaratik* ork,insan_yaratik* insan){
    aranandegerbul(json,orkyaratik_ad,"etki_degeri",ork->etki_degeri);
    aranandegerbul(json,orkyaratik_ad,"etki_turu",ork->etki_turu);
    aranandegerbul(json,orkyaratik_ad,"aciklama",ork->aciklama);
    aranandegerbul(json,insanyaratik_ad,"etki_degeri",insan->etki_degeri);
    aranandegerbul(json,insanyaratik_ad,"etki_turu",insan->etki_turu);
    aranandegerbul(json,insanyaratik_ad,"aciklama",insan->aciklama);
    insan->etki_degeri_float = atof(insan->etki_degeri);
    ork->etki_degeri_float = atof(ork->etki_degeri);
}
void senaryocek_insan(const char *json,insan_savas* insan){
    char deger[50];
    int deger_int=0;
    aranandegerbul_research(json, "insan_imparatorlugu", "birimler", "piyadeler", deger);
    insan->piyade_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "insan_imparatorlugu", "birimler", "okcular", deger);
    insan->okcu_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "insan_imparatorlugu", "birimler", "suvariler", deger);
    insan->suvari_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "insan_imparatorlugu", "birimler", "kusatma_makineleri", deger);
    insan->kusatmamak_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"insan_imparatorlugu","savunma_ustaligi",deger);
    insan->savunma_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"insan_imparatorlugu","saldiri_gelistirmesi",deger);
    insan->saldiri_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"insan_imparatorlugu","kusatma_ustaligi",deger);
    insan->kusatma_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"insan_imparatorlugu","elit_egitim",deger);
    insan->elit_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_senaryo(json, "insan_imparatorlugu", "kahramanlar", insan->kahraman, insan->kahraman1);
    aranandegerbul_senaryo(json,"insan_imparatorlugu","canavarlar",insan->canavar,insan->canavar1);
}
void senaryocek_ork(const char* json,ork_savas* ork){
    char deger[50];
    aranandegerbul_research(json, "ork_legi", "birimler", "ork_dovusculeri", deger);
    ork->ork_dovusculeri_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "ork_legi", "birimler", "troller", deger);
    ork->troller_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "ork_legi", "birimler", "varg_binicileri", deger);
    ork->varg_binicileri_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_research(json, "ork_legi", "birimler", "mizrakcilar", deger);
    ork->mizrakcilar_sayi = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"ork_legi","savunma_ustaligi",deger);
    ork->savunma_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"ork_legi","saldiri_gelistirmesi",deger);
    ork->saldiri_seviye = atoi(deger);
    deger[0] = '\0';
    aranandegerbul_ustalik(json,"ork_legi","elit_egitim",deger);
    ork->elit_seviye = atoi(deger);
    aranandegerbul_senaryo(json, "ork_legi", "kahramanlar",ork->kahraman,ork->kahraman1);
    aranandegerbul_senaryo(json,"ork_legi","canavarlar",ork->canavar,ork->canavar1);
}
void etkilenen_yukle_orkhero(ork_hero* ork){
 if(ork->aciklama[0]=='t'){
        if(ork->aciklama[1]=='r'){
            strcpy(ork->etkiledigi_birim,"troller");
        }
        else{
            strcpy(ork->etkiledigi_birim,"tum_birimler");
        }
 }
 else if(ork->aciklama[0]=='o'){
    strcpy(ork->etkiledigi_birim,"ork_dovusculeri");
 }
 else if(ork->aciklama[0]=='v'){
    strcpy(ork->etkiledigi_birim,"varg_binicileri");
 }
}
void etkilenen_yukle_insanyaratik(insan_yaratik* insan) {
if(insan->aciklama[0]=='p'){
    strcpy(insan->etkiledigi_birim,"piyadeler");
}else if(insan->aciklama[0]=='s'){
    strcpy(insan->etkiledigi_birim,"suvariler");
}else if(insan->aciklama[0]=='o'){
    strcpy(insan->etkiledigi_birim,"okcular");
}
}
void etkilenen_yukle_orkyaratik(orc_yaratik* ork){
if(ork->aciklama[0]=='t' || ork->aciklama[0]=='T'){
    strcpy(ork->etkiledigi_birim,"troller");
}
else if(ork->aciklama[0]=='o'){
    strcpy(ork->etkiledigi_birim,"ork_dovusculeri");
}
else if(ork->aciklama[0] =='v'){
    strcpy(ork->etkiledigi_birim,"varg_binicileri");
}
else if(ork->aciklama[0]=='s'){
    strcpy(ork->etkiledigi_birim,"suvariler");
}
else if(ork->aciklama[0]=='m'){
    strcpy(ork->etkiledigi_birim,"mizrakcilar");
}
}
void etkilenen_yukle_insanhero(insan_hero* insan){
if(insan->aciklama[0] =='p'){
    strcpy(insan->etkiledigi_birim,"piyadeler");
}
else if(insan->aciklama[0]=='k'){
    strcpy(insan->etkiledigi_birim,"kusatma_makineleri");
}else if(insan->aciklama[0]=='o'){
    strcpy(insan->etkiledigi_birim,"okcular");
}else if(insan->aciklama[0]=='s'){
    strcpy(insan->etkiledigi_birim,"suvariler");
}
}
void kritik_sans_yukle(const char * research,const char * heroes,const char *creatures,insan_imp *insan,insan_savas insanz,ork_savas orkz,ork_legi* ork,savas_hesapla* ork_deger,savas_hesapla *insan_deger){
    ustalik savunma,saldiri,elit_egitim,kusatma;
    bilgi_yukle_ustalik(research,&savunma,&saldiri,&elit_egitim,&kusatma);
    etkilenen_yukle_research(&elit_egitim.level_1);
    etkilenen_yukle_research(&elit_egitim.level_2);
    etkilenen_yukle_research(&elit_egitim.level_3);
    insan_yaratik karakurt;
    orc_yaratik ates_iblisi;
    insan_hero yavuz;
    ork_hero vrog_kafakiran;
    yaratik_yukle(creatures,"Ates_Iblisi","Karakurt",&ates_iblisi,&karakurt);
    hero_yukle(heroes, "Vrog_Kafakiran","Yavuz_Sultan_Selim",&vrog_kafakiran,&yavuz);
    etkilenen_yukle_insanyaratik(&karakurt);
    etkilenen_yukle_insanhero(&yavuz);
    etkilenen_yukle_orkhero(&vrog_kafakiran);
    if(strcmp(orkz.kahraman,"Vrog_Kafakiran") == 0)
    {
        if(strcmp(vrog_kafakiran.bonus_turu,"kritik_sans")==0){
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.kritik_sans+=(ork->varg_binicileri.kritik_sans*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.kritik_sans+=(ork->troller.kritik_sans*vrog_kafakiran.bonus_degeri_float)/100;
                }
            }
            if(strcmp(vrog_kafakiran.bonus_turu,"savunma")==0){
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.savunma+=(ork->troller.savunma*vrog_kafakiran.bonus_degeri_float)/100;
                }
            }
            if(strcmp(vrog_kafakiran.bonus_turu,"saldiri")==0)
            {
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*vrog_kafakiran.bonus_degeri_float)/100;
                }
                if(strcmp(vrog_kafakiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.saldiri+=(ork->troller.saldiri*vrog_kafakiran.bonus_degeri_float)/100;
                }
            }
    }
if(strcmp(insanz.canavar,"Karakurt")==0)
    {
        if(strcmp(karakurt.etkiledigi_birim,"kusatma_makineleri")== 0){
            if(strcmp(karakurt.etki_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*karakurt.etki_degeri_float)/100;
            }
        }
        if(strcmp(karakurt.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(karakurt.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*karakurt.etki_degeri_float)/100;
            }
        }
        if(strcmp(karakurt.etkiledigi_birim,"piyadeler")== 0){
            if(strcmp(karakurt.etki_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*karakurt.etki_degeri_float)/100;
            }
        }
        if(strcmp(karakurt.etkiledigi_birim,"okcular")==0)
        {
            if(strcmp(karakurt.etki_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*karakurt.etki_degeri_float)/100;
            }
            if(strcmp(karakurt.etki_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*karakurt.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(insanz.kahraman,"Yavuz_Sultan_Selim") == 0)
    {
        if(strcmp(yavuz.etkiledigi_birim,"kusatma_makineleri")==0){
            if(strcmp(yavuz.bonus_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*yavuz.bonus_degeri_float)/100;
            }
        }
        if(strcmp(yavuz.etkiledigi_birim,"suvariler")==0){
            if(strcmp(yavuz.bonus_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*yavuz.bonus_degeri_float)/100;
            }
        }
        if(strcmp(yavuz.etkiledigi_birim,"okcular")==0){
            if(strcmp(yavuz.bonus_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*yavuz.bonus_degeri_float)/100;
            }
        }
        if(strcmp(yavuz.etkiledigi_birim,"piyadeler")==0)
        {
            if(strcmp(yavuz.bonus_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*yavuz.bonus_degeri_float)/100;
            }
            if(strcmp(yavuz.bonus_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*yavuz.bonus_degeri_float)/100;
            }
        }
    }

        if(insanz.elit_seviye == 1){
        insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*elit_egitim.level_1.deger_float)/100;
        insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*elit_egitim.level_1.deger_float)/100;
        insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*elit_egitim.level_1.deger_float)/100;
        insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*elit_egitim.level_1.deger_float)/100;
    }
    if(insanz.elit_seviye == 2){
        insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*elit_egitim.level_2.deger_float)/100;
        insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*elit_egitim.level_2.deger_float)/100;
        insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*elit_egitim.level_2.deger_float)/100;
        insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*elit_egitim.level_2.deger_float)/100;
    }
    if(insanz.elit_seviye == 3){
        insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*elit_egitim.level_3.deger_float)/100;
        insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*elit_egitim.level_3.deger_float)/100;
        insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*elit_egitim.level_3.deger_float)/100;
        insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*elit_egitim.level_3.deger_float)/100;
    }
        if(orkz.elit_seviye == 1){
        ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*elit_egitim.level_1.deger_float)/100;
        ork->troller.kritik_sans+=(ork->troller.kritik_sans*elit_egitim.level_1.deger_float)/100;
        ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*elit_egitim.level_1.deger_float)/100;
        ork->varg_binicileri.kritik_sans+=(ork->varg_binicileri.kritik_sans*elit_egitim.level_1.deger_float)/100;
    }
        if(orkz.elit_seviye == 2){
        ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*elit_egitim.level_2.deger_float)/100;
        ork->troller.kritik_sans+=(ork->troller.kritik_sans*elit_egitim.level_2.deger_float)/100;
        ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*elit_egitim.level_2.deger_float)/100;
        ork->varg_binicileri.kritik_sans+=(ork->varg_binicileri.kritik_sans*elit_egitim.level_2.deger_float)/100;
    }
        if(orkz.elit_seviye == 3){

        ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*elit_egitim.level_3.deger_float)/100;
        ork->troller.kritik_sans+=(ork->troller.kritik_sans*elit_egitim.level_3.deger_float)/100;
        ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*elit_egitim.level_3.deger_float)/100;
        ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*elit_egitim.level_3.deger_float)/100;
    }
    ork_deger->saglik_total=ork->mizrakcilar.saglik*orkz.mizrakcilar_sayi+ork->ork_dovusculeri.saglik*orkz.ork_dovusculeri_sayi+ork->troller.saglik*orkz.troller_sayi+ork->varg_binicileri.saglik*orkz.varg_binicileri_sayi;
    insan_deger->saglik_total=insan->kusatma_makineleri.saglik*insanz.kusatmamak_sayi+insan->okcular.saglik*insanz.okcu_sayi+insan->piyadeler.saglik*insanz.piyade_sayi+insan->suvariler.saglik*insanz.suvari_sayi;
}
void etkilenen_yukle_research(ustalik* ustalik){
if(ustalik->level_1.aciklama[0]=='t'){
    strcpy(ustalik->level_1.etkilenen_birim,"tum_birimler");
}
if(ustalik->level_2.aciklama[0]=='t'){
    strcpy(ustalik->level_2.etkilenen_birim,"tum_birimler");
}
if(ustalik->level_3.aciklama[0]=='t'){
    strcpy(ustalik->level_3.etkilenen_birim,"tum_birimler");
}
if(ustalik->level_1.aciklama[0]=='k'){
    strcpy(ustalik->level_1.etkilenen_birim,"kusatma_makineleri");
}
if(ustalik->level_2.aciklama[0]=='k'){
    strcpy(ustalik->level_2.etkilenen_birim,"kusatma_makineleri");
}
if(ustalik->level_3.aciklama[0]=='k'){
    strcpy(ustalik->level_3.etkilenen_birim,"kusatma_makineleri");
}

}

void savas_hesapla_deger_ork(const char *research,const char *heroes,const char*creatures,insan_imp *insan,insan_savas insanz,ork_savas orkz ,ork_legi* ork,savas_hesapla* ork_deger){
    ustalik savunma,saldiri,elit_egitim,kusatma;
    bilgi_yukle_ustalik(research,&savunma,&saldiri,&elit_egitim,&kusatma);
    etkilenen_yukle_research(&savunma.level_1);
    etkilenen_yukle_research(&savunma.level_2);
    etkilenen_yukle_research(&savunma.level_3);
    etkilenen_yukle_research(&saldiri.level_1);
    etkilenen_yukle_research(&saldiri.level_2);
    etkilenen_yukle_research(&saldiri.level_3);
    etkilenen_yukle_research(&elit_egitim.level_1);
    etkilenen_yukle_research(&elit_egitim.level_2);
    etkilenen_yukle_research(&elit_egitim.level_3);
    etkilenen_yukle_research(&kusatma.level_1);
    etkilenen_yukle_research(&kusatma.level_2);
    etkilenen_yukle_research(&kusatma.level_3);
    insan_hero alparslan, fatih, yavuz,metehan,tugrul;
    ork_hero goruk_vahsi, thruk_kemikkiran, vrog_kafakiran,ugar_zalim;
    hero_yukle(heroes, "Goruk_Vahsi","Alparslan", &goruk_vahsi , &alparslan);
    hero_yukle(heroes, "Thruk_Kemikkiran","Fatih_Sultan_Mehmet", &thruk_kemikkiran,&fatih);
    hero_yukle(heroes, "Vrog_Kafakiran","Yavuz_Sultan_Selim",&vrog_kafakiran,&yavuz);
    hero_yukle(heroes, "Ugar_Zalim","Tugrul_Bey",&ugar_zalim,&tugrul);
    hero_yukle(heroes,"Ugar_Zalim","Mete_Han",&ugar_zalim,&metehan);
    etkilenen_yukle_orkhero(&goruk_vahsi);
    insan_yaratik ejderha, agri_dagi_devleri, tepegoz, karakurt, samur;
    orc_yaratik kara_troll,golge_kurtlari,camur_devleri,ates_iblisi,buz_devleri,makrog_savas_beyi;
    yaratik_yukle(creatures,"Kara_Troll","Ejderha",&kara_troll,&ejderha);
    yaratik_yukle(creatures,"Golge_Kurtlari","Agri_Dagi_Devleri",&golge_kurtlari,&agri_dagi_devleri);
    yaratik_yukle(creatures,"Camur_Devleri","Tepegoz",&camur_devleri,&tepegoz);
    yaratik_yukle(creatures,"Ates_Iblisi","Karakurt",&ates_iblisi,&karakurt);
    yaratik_yukle(creatures,"Buz_Devleri","Samur",&buz_devleri,&samur);
    yaratik_yukle(creatures,"Makrog_Savas_Beyi","Samur",&makrog_savas_beyi,&samur);
    etkilenen_yukle_insanyaratik(&ejderha);
    etkilenen_yukle_insanyaratik(&agri_dagi_devleri);
    etkilenen_yukle_insanyaratik(&tepegoz);
    etkilenen_yukle_insanyaratik(&karakurt);
    etkilenen_yukle_insanyaratik(&samur);
    etkilenen_yukle_orkyaratik(&kara_troll);
    etkilenen_yukle_orkyaratik(&golge_kurtlari);
    etkilenen_yukle_orkyaratik(&camur_devleri);
    etkilenen_yukle_orkyaratik(&ates_iblisi);
    etkilenen_yukle_orkyaratik(&buz_devleri);
    etkilenen_yukle_orkyaratik(&makrog_savas_beyi);
    etkilenen_yukle_insanhero(&alparslan);
    etkilenen_yukle_insanhero(&fatih);
    etkilenen_yukle_insanhero(&metehan);
    etkilenen_yukle_insanhero(&yavuz);
    etkilenen_yukle_insanhero(&tugrul);
    etkilenen_yukle_orkhero(&goruk_vahsi);
    etkilenen_yukle_orkhero(&thruk_kemikkiran);
    etkilenen_yukle_orkhero(&vrog_kafakiran);
    etkilenen_yukle_orkhero(&ugar_zalim);
    if(strcmp(orkz.canavar,"Kara_Troll") == 0){
        if(strcmp(kara_troll.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(kara_troll.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*kara_troll.etki_degeri_float)/100;
            }
        }
        if(strcmp(kara_troll.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(kara_troll.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*kara_troll.etki_degeri_float)/100;
            }
        }
        if(strcmp(kara_troll.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(kara_troll.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*kara_troll.etki_degeri_float)/100;
            }
        }
        if(strcmp(kara_troll.etkiledigi_birim,"troller")== 0){
            if(strcmp(kara_troll.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*kara_troll.etki_degeri_float)/100;
            }
            if(strcmp(kara_troll.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*kara_troll.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.canavar,"Golge_Kurtlari") == 0){
        if(strcmp(golge_kurtlari.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(golge_kurtlari.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*golge_kurtlari.etki_degeri_float)/100;
            }
        }
        if(strcmp(golge_kurtlari.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(golge_kurtlari.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*golge_kurtlari.etki_degeri_float)/100;
            }
        }
        if(strcmp(golge_kurtlari.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(golge_kurtlari.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*golge_kurtlari.etki_degeri_float)/100;
            }
        }
        if(strcmp(golge_kurtlari.etkiledigi_birim,"troller")== 0){
            if(strcmp(golge_kurtlari.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*golge_kurtlari.etki_degeri_float)/100;
            }
            if(strcmp(golge_kurtlari.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*golge_kurtlari.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.canavar,"Camur_Devleri") == 0){
        if(strcmp(camur_devleri.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(camur_devleri.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*camur_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(camur_devleri.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(camur_devleri.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*camur_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(camur_devleri.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(camur_devleri.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*camur_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(camur_devleri.etkiledigi_birim,"troller")== 0){
            if(strcmp(camur_devleri.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*camur_devleri.etki_degeri_float)/100;
            }
            if(strcmp(camur_devleri.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*camur_devleri.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.canavar,"Makrog_Savas_Beyi") == 0){
        if(strcmp(makrog_savas_beyi.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(makrog_savas_beyi.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*makrog_savas_beyi.etki_degeri_float)/100;
            }
        }
        if(strcmp(makrog_savas_beyi.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(makrog_savas_beyi.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*makrog_savas_beyi.etki_degeri_float)/100;
            }
        }
        if(strcmp(makrog_savas_beyi.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(makrog_savas_beyi.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*makrog_savas_beyi.etki_degeri_float)/100;
            }
        }
        if(strcmp(makrog_savas_beyi.etkiledigi_birim,"troller")== 0){
            if(strcmp(makrog_savas_beyi.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*makrog_savas_beyi.etki_degeri_float)/100;
            }
            if(strcmp(makrog_savas_beyi.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*makrog_savas_beyi.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.canavar,"Ates_Iblisi") == 0){
        if(strcmp(ates_iblisi.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(ates_iblisi.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*ates_iblisi.etki_degeri_float)/100;
            }
        }
        if(strcmp(ates_iblisi.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(ates_iblisi.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*ates_iblisi.etki_degeri_float)/100;
            }
        }
            if(strcmp(ates_iblisi.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(ates_iblisi.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*ates_iblisi.etki_degeri_float)/100;
            }
        }
        if(strcmp(ates_iblisi.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(ates_iblisi.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*ates_iblisi.etki_degeri_float)/100;
            }
        }
        if(strcmp(ates_iblisi.etkiledigi_birim,"troller")== 0){
            if(strcmp(ates_iblisi.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*ates_iblisi.etki_degeri_float)/100;
            }
            if(strcmp(ates_iblisi.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*ates_iblisi.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.canavar,"Buz_Devleri") == 0){
        if(strcmp(buz_devleri.etkiledigi_birim,"ork_dovusculeri")== 0){
            if(strcmp(buz_devleri.etki_turu,"savunma") == 0){
                ork->ork_dovusculeri.savunma=ork->ork_dovusculeri.savunma+(ork->ork_dovusculeri.savunma*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"saldiri") == 0){
                ork->ork_dovusculeri.saldiri=ork->ork_dovusculeri.saldiri+(ork->ork_dovusculeri.saldiri*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"kritik_sans") == 0){
                ork->ork_dovusculeri.kritik_sans=ork->ork_dovusculeri.kritik_sans+(ork->ork_dovusculeri.kritik_sans*buz_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(buz_devleri.etkiledigi_birim,"mizrakcilar")== 0){
            if(strcmp(buz_devleri.etki_turu,"savunma") == 0){
                ork->mizrakcilar.savunma=ork->mizrakcilar.savunma+(ork->mizrakcilar.savunma*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"saldiri") == 0){
                ork->mizrakcilar.saldiri=ork->mizrakcilar.saldiri+(ork->mizrakcilar.saldiri*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"kritik_sans") == 0){
                ork->mizrakcilar.kritik_sans=ork->mizrakcilar.kritik_sans+(ork->mizrakcilar.kritik_sans*buz_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(buz_devleri.etkiledigi_birim,"varg_binicileri")== 0){
            if(strcmp(buz_devleri.etki_turu,"savunma") == 0){
                ork->varg_binicileri.savunma=ork->varg_binicileri.savunma+(ork->varg_binicileri.savunma*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"saldiri") == 0){
                ork->varg_binicileri.saldiri=ork->varg_binicileri.saldiri+(ork->varg_binicileri.saldiri*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"kritik_sans") == 0){
                ork->varg_binicileri.kritik_sans=ork->varg_binicileri.kritik_sans+(ork->varg_binicileri.kritik_sans*buz_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(buz_devleri.etkiledigi_birim,"troller")== 0){
            if(strcmp(buz_devleri.etki_turu,"savunma") == 0){
                ork->troller.savunma=ork->troller.savunma+(ork->troller.savunma*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"saldiri") == 0){
                ork->troller.saldiri=ork->troller.saldiri+(ork->troller.saldiri*buz_devleri.etki_degeri_float)/100;
            }
            if(strcmp(buz_devleri.etki_turu,"kritik_sans") == 0){
                ork->troller.kritik_sans=ork->troller.kritik_sans+(ork->troller.kritik_sans*buz_devleri.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(orkz.kahraman,"Goruk_Vahsi") == 0){
            if(strcmp(goruk_vahsi.bonus_turu,"kritik_sans")==0){
                if(strcmp(goruk_vahsi.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.kritik_sans+=(ork->varg_binicileri.kritik_sans*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.kritik_sans+=(ork->troller.kritik_sans*goruk_vahsi.bonus_degeri_float)/100;
                }
            }
            if(strcmp(goruk_vahsi.bonus_turu,"savunma")==0){
                if(strcmp(goruk_vahsi.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.savunma+=(ork->troller.savunma*goruk_vahsi.bonus_degeri_float)/100;
                }
            }
            if(strcmp(goruk_vahsi.bonus_turu,"saldiri")==0)
            {
                if(strcmp(goruk_vahsi.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*goruk_vahsi.bonus_degeri_float)/100;
                }
                if(strcmp(goruk_vahsi.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.saldiri+=(ork->troller.saldiri*goruk_vahsi.bonus_degeri_float)/100;
                }
            }
        }
    if(strcmp(orkz.kahraman,"Thruk_Kemikkiran") == 0)
    {
            if(strcmp(thruk_kemikkiran.bonus_turu,"kritik_sans")==0){
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.kritik_sans+=(ork->ork_dovusculeri.kritik_sans*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.kritik_sans+=(ork->mizrakcilar.kritik_sans*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.kritik_sans+=(ork->varg_binicileri.kritik_sans*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.kritik_sans+=(ork->troller.kritik_sans*thruk_kemikkiran.bonus_degeri_float)/100;
                }
            }
            if(strcmp(thruk_kemikkiran.bonus_turu,"savunma")==0){
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.savunma+=(ork->troller.savunma*thruk_kemikkiran.bonus_degeri_float)/100;
                }
            }
            if(strcmp(thruk_kemikkiran.bonus_turu,"saldiri")==0)
            {
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0)
                {
                    ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"mizrakcilar")==0)
                {
                    ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"varg_binicileri")==0)
                {
                    ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*thruk_kemikkiran.bonus_degeri_float)/100;
                }
                if(strcmp(thruk_kemikkiran.etkiledigi_birim,"troller")==0)
                {
                    ork->troller.saldiri+=(ork->troller.saldiri*thruk_kemikkiran.bonus_degeri_float)/100;
                }
            }
    }

    if(strcmp(orkz.kahraman,"Ugar_Zalim") == 0){
        ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*ugar_zalim.bonus_degeri_float)/100;
        ork->troller.savunma+=(ork->troller.savunma*ugar_zalim.bonus_degeri_float)/100;
        ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*ugar_zalim.bonus_degeri_float)/100;
        ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*ugar_zalim.bonus_degeri_float)/100;
    }
    if(orkz.savunma_seviye == 1){
        ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*savunma.level_1.deger_float)/100;
        ork->troller.savunma+=(ork->troller.savunma*savunma.level_1.deger_float)/100;
        ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*savunma.level_1.deger_float)/100;
        ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*savunma.level_1.deger_float)/100;
    }
      if(orkz.savunma_seviye == 2){
        ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*savunma.level_2.deger_float)/100;
        ork->troller.savunma+=(ork->troller.savunma*savunma.level_2.deger_float)/100;
        ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*savunma.level_2.deger_float)/100;
        ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*savunma.level_2.deger_float)/100;
    }
      if(orkz.savunma_seviye == 3){
        ork->ork_dovusculeri.savunma+=(ork->ork_dovusculeri.savunma*savunma.level_3.deger_float)/100;
        ork->troller.savunma+=(ork->troller.savunma*savunma.level_3.deger_float)/100;
        ork->mizrakcilar.savunma+=(ork->mizrakcilar.savunma*savunma.level_3.deger_float)/100;
        ork->varg_binicileri.savunma+=(ork->varg_binicileri.savunma*savunma.level_3.deger_float)/100;
    }
    if(orkz.saldiri_seviye == 1){
        ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*saldiri.level_1.deger_float)/100;
        ork->troller.saldiri+=(ork->troller.saldiri*saldiri.level_1.deger_float)/100;
        ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*saldiri.level_1.deger_float)/100;
        ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*saldiri.level_1.deger_float)/100;
    }
        if(orkz.saldiri_seviye == 2){
        ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*saldiri.level_2.deger_float)/100;
        ork->troller.saldiri+=(ork->troller.saldiri*saldiri.level_2.deger_float)/100;
        ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*saldiri.level_2.deger_float)/100;
        ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*saldiri.level_2.deger_float)/100;
    }
        if(orkz.saldiri_seviye == 3){
        ork->ork_dovusculeri.saldiri+=(ork->ork_dovusculeri.saldiri*saldiri.level_3.deger_float)/100;
        ork->troller.saldiri+=(ork->troller.saldiri*saldiri.level_3.deger_float)/100;
        ork->mizrakcilar.saldiri+=(ork->mizrakcilar.saldiri*saldiri.level_3.deger_float)/100;
        ork->varg_binicileri.saldiri+=(ork->varg_binicileri.saldiri*saldiri.level_3.deger_float)/100;
    }

    ork->mizrakcilar.saldiri_kritik=ork->mizrakcilar.saldiri*1.5;
    ork->ork_dovusculeri.saldiri_kritik=ork->ork_dovusculeri.saldiri*1.5;
    ork->troller.saldiri_kritik=ork->troller.saldiri*1.5;
    ork->varg_binicileri.saldiri_kritik=ork->varg_binicileri.saldiri*1.5;
    }
void savas_hesapla_deger_insan(const char *research,const char *heroes,const char*creatures,insan_imp *insan,insan_savas insanz,ork_savas orkz ,ork_legi* ork,savas_hesapla* insan_deger){
     ustalik savunma,saldiri,elit_egitim,kusatma;
    bilgi_yukle_ustalik(research,&savunma,&saldiri,&elit_egitim,&kusatma);
    etkilenen_yukle_research(&savunma.level_1);
    etkilenen_yukle_research(&savunma.level_2);
    etkilenen_yukle_research(&savunma.level_3);
    etkilenen_yukle_research(&saldiri.level_1);
    etkilenen_yukle_research(&saldiri.level_2);
    etkilenen_yukle_research(&saldiri.level_3);
    etkilenen_yukle_research(&elit_egitim.level_1);
    etkilenen_yukle_research(&elit_egitim.level_2);
    etkilenen_yukle_research(&elit_egitim.level_3);
    etkilenen_yukle_research(&kusatma.level_1);
    etkilenen_yukle_research(&kusatma.level_2);
    etkilenen_yukle_research(&kusatma.level_3);
    insan_hero alparslan, fatih, yavuz,metehan,tugrul;
    ork_hero goruk_vahsi, thruk_kemikkiran, vrog_kafakiran,ugar_zalim;
    hero_yukle(heroes, "Goruk_Vahsi","Alparslan", &goruk_vahsi , &alparslan);
    hero_yukle(heroes, "Thruk_Kemikkiran","Fatih_Sultan_Mehmet", &thruk_kemikkiran,&fatih);
    hero_yukle(heroes, "Vrog_Kafakiran","Yavuz_Sultan_Selim",&vrog_kafakiran,&yavuz);
    hero_yukle(heroes, "Ugar_Zalim","Tugrul_Bey",&ugar_zalim,&tugrul);
    hero_yukle(heroes,"Ugar_Zalim","Mete_Han",&ugar_zalim,&metehan);
    etkilenen_yukle_orkhero(&goruk_vahsi);
    insan_yaratik ejderha, agri_dagi_devleri, tepegoz, karakurt, samur;
    orc_yaratik kara_troll,golge_kurtlari,camur_devleri,ates_iblisi,buz_devleri,makrog_savas_beyi;
    yaratik_yukle(creatures,"Kara_Troll","Ejderha",&kara_troll,&ejderha);
    yaratik_yukle(creatures,"Golge_Kurtlari","Agri_Dagi_Devleri",&golge_kurtlari,&agri_dagi_devleri);
    yaratik_yukle(creatures,"Camur_Devleri","Tepegoz",&camur_devleri,&tepegoz);
    yaratik_yukle(creatures,"Ates_Iblisi","Karakurt",&ates_iblisi,&karakurt);
    yaratik_yukle(creatures,"Buz_Devleri","Samur",&buz_devleri,&samur);
    yaratik_yukle(creatures,"Makrog_Savas_Beyi","Samur",&makrog_savas_beyi,&samur);
    etkilenen_yukle_insanyaratik(&ejderha);
    etkilenen_yukle_insanyaratik(&agri_dagi_devleri);
    etkilenen_yukle_insanyaratik(&tepegoz);
    etkilenen_yukle_insanyaratik(&karakurt);
    etkilenen_yukle_insanyaratik(&samur);
    etkilenen_yukle_orkyaratik(&kara_troll);
    etkilenen_yukle_orkyaratik(&golge_kurtlari);
    etkilenen_yukle_orkyaratik(&camur_devleri);
    etkilenen_yukle_orkyaratik(&ates_iblisi);
    etkilenen_yukle_orkyaratik(&buz_devleri);
    etkilenen_yukle_orkyaratik(&makrog_savas_beyi);
    etkilenen_yukle_insanhero(&alparslan);
    etkilenen_yukle_insanhero(&fatih);
    etkilenen_yukle_insanhero(&metehan);
    etkilenen_yukle_insanhero(&yavuz);
    etkilenen_yukle_insanhero(&tugrul);
    etkilenen_yukle_orkhero(&goruk_vahsi);
    etkilenen_yukle_orkhero(&thruk_kemikkiran);
    etkilenen_yukle_orkhero(&vrog_kafakiran);
    etkilenen_yukle_orkhero(&ugar_zalim);
    if(strcmp(insanz.canavar,"Tepegoz") == 0)
    {
        if(strcmp(tepegoz.etkiledigi_birim,"kusatma_makineleri")== 0){
            if(strcmp(tepegoz.etki_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*tepegoz.etki_degeri_float)/100;
            }
        }
        if(strcmp(tepegoz.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(tepegoz.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*tepegoz.etki_degeri_float)/100;
            }
        }
        if(strcmp(tepegoz.etkiledigi_birim,"piyadeler")== 0){
            if(strcmp(tepegoz.etki_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*tepegoz.etki_degeri_float)/100;
            }
        }
        if(strcmp(tepegoz.etkiledigi_birim,"okcular")==0)
        {
            if(strcmp(tepegoz.etki_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*tepegoz.etki_degeri_float)/100;
            }
            if(strcmp(tepegoz.etki_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*tepegoz.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(insanz.canavar,"Agri_Dagi_Devleri")==0)
    {
        if(strcmp(agri_dagi_devleri.etkiledigi_birim,"kusatma_makineleri")== 0){
            if(strcmp(agri_dagi_devleri.etki_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*agri_dagi_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(agri_dagi_devleri.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(agri_dagi_devleri.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*agri_dagi_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(agri_dagi_devleri.etkiledigi_birim,"piyadeler")== 0){
            if(strcmp(agri_dagi_devleri.etki_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*agri_dagi_devleri.etki_degeri_float)/100;
            }
        }
        if(strcmp(agri_dagi_devleri.etkiledigi_birim,"okcular")==0)
        {
            if(strcmp(agri_dagi_devleri.etki_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*agri_dagi_devleri.etki_degeri_float)/100;
            }
            if(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*agri_dagi_devleri.etki_degeri_float)/100;
            }
        }
    }

    if(strcmp(insanz.canavar,"Ejderha") == 0)
    {
        if(strcmp(ejderha.etkiledigi_birim,"kusatma_makineleri")== 0){
            if(strcmp(ejderha.etki_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*ejderha.etki_degeri_float)/100;
            }
        }
        if(strcmp(ejderha.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(ejderha.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*ejderha.etki_degeri_float)/100;
            }
        }
        if(strcmp(ejderha.etkiledigi_birim,"piyadeler")== 0){
            if(strcmp(ejderha.etki_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*ejderha.etki_degeri_float)/100;
            }
        }
        if(strcmp(ejderha.etkiledigi_birim,"okcular")==0)
        {
            if(strcmp(ejderha.etki_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*ejderha.etki_degeri_float)/100;
            }
            if(strcmp(ejderha.etki_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*ejderha.etki_degeri_float)/100;
            }
        }
    }

    if(strcmp(insanz.canavar,"Samur")== 0)
    {
        if(strcmp(samur.etkiledigi_birim,"kusatma_makineleri")== 0){
            if(strcmp(samur.etki_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*samur.etki_degeri_float)/100;
            }
        }
        if(strcmp(samur.etkiledigi_birim,"suvariler")== 0){
            if(strcmp(samur.etki_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*samur.etki_degeri_float)/100;
            }
        }
        if(strcmp(samur.etkiledigi_birim,"piyadeler")== 0){
            if(strcmp(samur.etki_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*samur.etki_degeri_float)/100;
            }
        }
        if(strcmp(samur.etkiledigi_birim,"okcular")==0)
        {
            if(strcmp(samur.etki_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*samur.etki_degeri_float)/100;
            }
            if(strcmp(samur.etki_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*samur.etki_degeri_float)/100;
            }
        }
    }
    if(strcmp(insanz.kahraman,"Alparslan") == 0)
    {
        if(strcmp(alparslan.etkiledigi_birim,"kusatma_makineleri")==0){
            if(strcmp(alparslan.bonus_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*alparslan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(alparslan.etkiledigi_birim,"suvariler")==0){
            if(strcmp(alparslan.bonus_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*alparslan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(alparslan.etkiledigi_birim,"okcular")==0){
            if(strcmp(alparslan.bonus_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*alparslan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(alparslan.etkiledigi_birim,"piyadeler")==0)
        {
            if(strcmp(alparslan.bonus_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*alparslan.bonus_degeri_float)/100;
            }
            if(strcmp(alparslan.bonus_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*alparslan.bonus_degeri_float)/100;
            }
        }
    }
    if(strcmp(insanz.kahraman,"Fatih_Sultan_Mehmet") == 0){
        if(strcmp(fatih.etkiledigi_birim,"kusatma_makineleri")==0){
            if(strcmp(fatih.bonus_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*fatih.bonus_degeri_float)/100;
            }
        }
        if(strcmp(fatih.etkiledigi_birim,"suvariler")==0){
            if(strcmp(fatih.bonus_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*fatih.bonus_degeri_float)/100;
            }
        }
        if(strcmp(fatih.etkiledigi_birim,"okcular")==0){
            if(strcmp(fatih.bonus_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*fatih.bonus_degeri_float)/100;
            }
        }
        if(strcmp(fatih.etkiledigi_birim,"piyadeler")==0)
        {
            if(strcmp(fatih.bonus_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*fatih.bonus_degeri_float)/100;
            }
            if(strcmp(fatih.bonus_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*fatih.bonus_degeri_float)/100;
            }
        }
    }
    if(strcmp(insanz.kahraman,"Mete_Han") == 0)
    {
        if(strcmp(metehan.etkiledigi_birim,"kusatma_makineleri")==0){
            if(strcmp(metehan.bonus_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*metehan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(metehan.etkiledigi_birim,"suvariler")==0){
            if(strcmp(metehan.bonus_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*metehan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(metehan.etkiledigi_birim,"okcular")==0){
            if(strcmp(metehan.bonus_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*metehan.bonus_degeri_float)/100;
            }
        }
        if(strcmp(metehan.etkiledigi_birim,"piyadeler")==0)
        {
            if(strcmp(metehan.bonus_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*metehan.bonus_degeri_float)/100;
            }
            if(strcmp(metehan.bonus_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*metehan.bonus_degeri_float)/100;
            }
        }
    }

        if(strcmp(insanz.kahraman,"Tugrul_Bey") == 0)
        {
        if(strcmp(tugrul.etkiledigi_birim,"kusatma_makineleri")==0){
            if(strcmp(tugrul.bonus_turu,"savunma") == 0){
                insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"saldiri") == 0){
                insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"kritik_sans") == 0){
                insan->kusatma_makineleri.kritik_sans=insan->kusatma_makineleri.kritik_sans+(insan->kusatma_makineleri.kritik_sans*tugrul.bonus_degeri_float)/100;
            }
        }
        if(strcmp(tugrul.etkiledigi_birim,"suvariler")==0){
            if(strcmp(tugrul.bonus_turu,"savunma") == 0){
                insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"saldiri") == 0){
                insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"kritik_sans") == 0){
                insan->suvariler.kritik_sans=insan->suvariler.kritik_sans+(insan->suvariler.kritik_sans*tugrul.bonus_degeri_float)/100;
            }
        }
        if(strcmp(tugrul.etkiledigi_birim,"okcular")==0){
            if(strcmp(tugrul.bonus_turu,"savunma") == 0){
                insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"saldiri") == 0){
                insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"kritik_sans") == 0){
                insan->okcular.kritik_sans=insan->okcular.kritik_sans+(insan->okcular.kritik_sans*tugrul.bonus_degeri_float)/100;
            }
        }
        if(strcmp(tugrul.etkiledigi_birim,"piyadeler")==0)
        {
            if(strcmp(tugrul.bonus_turu,"savunma") == 0){
                insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"saldiri") == 0){
                insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*tugrul.bonus_degeri_float)/100;
            }
            if(strcmp(tugrul.bonus_turu,"kritik_sans") == 0){
                insan->piyadeler.kritik_sans=insan->piyadeler.kritik_sans+(insan->piyadeler.kritik_sans*tugrul.bonus_degeri_float)/100;
            }
        }
        }
    if(insanz.savunma_seviye==1){
    insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*savunma.level_1.deger_float)/100;
    insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*savunma.level_1.deger_float)/100;
    insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*savunma.level_1.deger_float)/100;
    insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*savunma.level_1.deger_float)/100;
    }
    if(insanz.savunma_seviye == 2){
        insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*savunma.level_2.deger_float)/100;
        insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*savunma.level_2.deger_float)/100;
        insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*savunma.level_2.deger_float)/100;
        insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*savunma.level_2.deger_float)/100;
    }
    if(insanz.savunma_seviye == 3){
        insan->okcular.savunma=insan->okcular.savunma+(insan->okcular.savunma*savunma.level_3.deger_float)/100;
        insan->kusatma_makineleri.savunma=insan->kusatma_makineleri.savunma+(insan->kusatma_makineleri.savunma*savunma.level_3.deger_float)/100;
        insan->piyadeler.savunma=insan->piyadeler.savunma+(insan->piyadeler.savunma*savunma.level_3.deger_float)/100;
        insan->suvariler.savunma=insan->suvariler.savunma+(insan->suvariler.savunma*savunma.level_3.deger_float)/100;
    }
    if(insanz.saldiri_seviye == 1){
        insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*saldiri.level_1.deger_float)/100;
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*saldiri.level_2.deger_float)/100;
        insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*saldiri.level_1.deger_float)/100;
        insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*saldiri.level_1.deger_float)/100;
    }
    if(insanz.saldiri_seviye == 2){
        insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*saldiri.level_2.deger_float)/100;
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*saldiri.level_2.deger_float)/100;
        insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*saldiri.level_2.deger_float)/100;
        insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*saldiri.level_2.deger_float)/100;
    }
    if(insanz.saldiri_seviye == 3){
        insan->okcular.saldiri=insan->okcular.saldiri+(insan->okcular.saldiri*saldiri.level_3.deger_float)/100;
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*saldiri.level_3.deger_float)/100;
        insan->suvariler.saldiri=insan->suvariler.saldiri+(insan->suvariler.saldiri*saldiri.level_3.deger_float)/100;
        insan->piyadeler.saldiri=insan->piyadeler.saldiri+(insan->piyadeler.saldiri*saldiri.level_3.deger_float)/100;
    }
    if(insanz.kusatma_seviye == 1){
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*kusatma.level_1.deger_float)/100;
    }
    if(insanz.kusatma_seviye == 2){
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*kusatma.level_2.deger_float)/100;
    }
    if(insanz.kusatma_seviye == 3){
        insan->kusatma_makineleri.saldiri=insan->kusatma_makineleri.saldiri+(insan->kusatma_makineleri.saldiri*kusatma.level_3.deger_float)/100;
    }
   insan->kusatma_makineleri.saldiri_kritik=insan->kusatma_makineleri.saldiri*1.5;
    insan->okcular.saldiri_kritik=insan->okcular.saldiri*1.5;
    insan->piyadeler.saldiri_kritik=insan->piyadeler.saldiri*1.5;
    insan->suvariler.saldiri_kritik=insan->piyadeler.saldiri*1.5;
}
void insan_birim_azalt(insan_savas* insanz,insan_imp* insan,savas_hesapla* insan_deger,float insanlarin_yedigi_hasar,char name[50]){
    float piyadelerin_yedigi_hasar,okcularin_yedigi_hasar,suvarilerin_yedigi_hasar,kusatmamak_yedigi_hasar,piyadeler_saglik,okcular_saglik,suvariler_saglik,kusatmamak_saglik;
    piyadelerin_yedigi_hasar=insan->piyadeler.savunma*(insanz->piyade_sayi/insan_deger->savunma_total)*insanlarin_yedigi_hasar;
    okcularin_yedigi_hasar =insan->okcular.savunma*(insanz->okcu_sayi/insan_deger->savunma_total)*insanlarin_yedigi_hasar;
    suvarilerin_yedigi_hasar = insan->suvariler.savunma*(insanz->suvari_sayi/insan_deger->savunma_total)*insanlarin_yedigi_hasar;
    kusatmamak_yedigi_hasar = insan->kusatma_makineleri.savunma*(insanz->kusatmamak_sayi/insan_deger->savunma_total)*insanlarin_yedigi_hasar;
    piyadeler_saglik = insanz->piyade_sayi*insan->piyadeler.saglik;
    okcular_saglik = insanz->okcu_sayi*insan->okcular.saglik;
    suvariler_saglik = insanz->suvari_sayi*insan->suvariler.saglik;
    kusatmamak_saglik = insanz->kusatmamak_sayi*insan->kusatma_makineleri.saglik;

    if(strcmp(name,"piyadeler")==0){
        int olen_piyade = piyadelerin_yedigi_hasar/insan->piyadeler.saglik;
        insanz->piyade_sayi=insanz->piyade_sayi-(olen_piyade);
    }
    else if(strcmp(name,"okcular")==0){
        int olen_okcu = okcularin_yedigi_hasar/insan->okcular.saglik;
        insanz->okcu_sayi = insanz->okcu_sayi - olen_okcu;
    }
    else if(strcmp(name,"suvariler")==0){
        int olen_suvari = suvarilerin_yedigi_hasar/insan->suvariler.saglik;
        insanz->suvari_sayi = insanz->suvari_sayi -olen_suvari;
    }
    else if(strcmp(name,"kusatma_makinesi")==0){
        int olen_kusatmamak = kusatmamak_yedigi_hasar/insan->kusatma_makineleri.saglik;
        insanz->kusatmamak_sayi = insanz->kusatmamak_sayi-olen_kusatmamak;
    }
    if(insanz->piyade_sayi<=0){
        insanz->piyade_sayi=0;
    }
    if(insanz->okcu_sayi<=0){
        insanz->okcu_sayi=0;
    }
    if(insanz->suvari_sayi<=0){
        insanz->suvari_sayi=0;
    }
    if(insanz->kusatmamak_sayi<=0){
        insanz->kusatmamak_sayi=0;
    }
}
void ork_birim_azalt(ork_savas* orkz,ork_legi* ork,savas_hesapla* ork_deger,float orklarin_yedigi_hasar,char name[50]){
    float orkdovusculerinin_yedigi_hasar,mizrakcilarin_yedigi_hasar,vargbinicileri_yedigi_hasar,trollerin_yedigi_hasar,orkdovusculeri_saglik,mizrakcilar_saglik,vargbinicisi_saglik,troller_saglik;
    orkdovusculerinin_yedigi_hasar=ork->ork_dovusculeri.savunma*(orkz->ork_dovusculeri_sayi/ork_deger->savunma_total)*orklarin_yedigi_hasar;
    mizrakcilarin_yedigi_hasar =ork->mizrakcilar.savunma*(orkz->mizrakcilar_sayi/ork_deger->savunma_total)*orklarin_yedigi_hasar;
    vargbinicileri_yedigi_hasar = ork->varg_binicileri.savunma*(orkz->varg_binicileri_sayi/ork_deger->savunma_total)*orklarin_yedigi_hasar;
    trollerin_yedigi_hasar = (ork->troller.savunma*orkz->troller_sayi)/(ork_deger->savunma_total)*(orklarin_yedigi_hasar);
    orkdovusculeri_saglik = orkz->ork_dovusculeri_sayi*ork->ork_dovusculeri.saglik;
    mizrakcilar_saglik = orkz->mizrakcilar_sayi*ork->mizrakcilar.saglik;
    vargbinicisi_saglik = orkz->varg_binicileri_sayi*ork->varg_binicileri.saglik;
    troller_saglik = orkz->troller_sayi*ork->troller.saglik;
    if(strcmp(name,"ork_dovusculeri")==0){
            int olen_orkdovuscu = orkdovusculerinin_yedigi_hasar/ork->ork_dovusculeri.saglik;
            orkz->ork_dovusculeri_sayi=orkz->ork_dovusculeri_sayi-(olen_orkdovuscu);
    }
    else if(strcmp(name,"varg_binicileri")==0){
        int olen_vargbinici = vargbinicileri_yedigi_hasar/ork->varg_binicileri.saglik;
        orkz->varg_binicileri_sayi = orkz->varg_binicileri_sayi - olen_vargbinici;
    }
    else if(strcmp(name,"mizrakcilar")==0){

        int olen_mizrakci = mizrakcilarin_yedigi_hasar/ork->mizrakcilar.saglik;
        orkz->mizrakcilar_sayi = orkz->mizrakcilar_sayi - olen_mizrakci;
    }
    else if(strcmp(name,"troller")==0){
        int olen_troller = trollerin_yedigi_hasar/ork->troller.saglik;
        orkz->troller_sayi = orkz->troller_sayi-olen_troller;
    }
    if(orkz->ork_dovusculeri_sayi<=0){
        orkz->ork_dovusculeri_sayi=0;
    }
    if(orkz->varg_binicileri_sayi<=0){
        orkz->varg_binicileri_sayi=0;
    }
    if(orkz->mizrakcilar_sayi<=0){
        orkz->mizrakcilar_sayi=0;
    }
    if(orkz->troller_sayi<0){
        orkz->troller_sayi=0;
    }

}
void senaryoyu_ciz(savas_hesapla *insan_deger,savas_hesapla*ork_deger,Birim* kusatmamak,Birim* piyade,Birim* okcu,Birim* trol,Birim* ork_dovuscusu,Birim* mizrakci,Birim* suvari,Birim* varg_binicisi,insan_savas* insan,ork_savas*ork,Texture2D sarmasik,Texture2D baslangic_arkaplan,bool ork_kazandi_mi){
    int maximum_birim = 100;
while (!WindowShouldClose()) {
        int duzleme_ekleX = (ekran_genislik - (kare_boyut * 20)) / 2;
        int duzleme_ekleY = (ekran_uzunluk - (kare_boyut * 20)) / 2;
        BeginDrawing();
        ClearBackground(RAYWHITE);
                DrawTexture(baslangic_arkaplan, 0, 0, WHITE);
                DrawTexture(sarmasik, 170,70, WHITE);
                DrawTexture(sarmasik, 1040,70, WHITE);
        // ızgara çiz
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                int x = duzleme_ekleX + j * kare_boyut;
                int y = duzleme_ekleY + i * kare_boyut;

                Color boya;
                if (j < 10)
                {
                    boya = BROWN;
                }
                else
                {
                    boya = DARKGREEN;
                }
                DrawRectangle(x, y, kare_boyut, kare_boyut, boya);
                DrawRectangleLines(x, y, kare_boyut, kare_boyut, BLACK);
            }
        }


static int mizrakci_rastgele_x[100];
static int mizrakci_rastgele_y[100];
static bool pozisyon_atama_mizrakci = false;
Vector2 mizrakci_yer = (Vector2){
    duzleme_ekleX + mizrakci->yer.x ,
    duzleme_ekleY + mizrakci->yer.y
};
int kalan_birim_mizrakci = mizrakci->birim_sayisi;
if (!pozisyon_atama_mizrakci) {
    for (int i = 0; i < 100; i++) {
        mizrakci_rastgele_x[i] = rand() % 10 + 10 ;
        mizrakci_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_mizrakci = true;
}
int sayac_mizrakci = 0;
Vector2 anlik_mizrakci_yer;
while (kalan_birim_mizrakci > 0) {
    int cizilecek_birimler;
if (kalan_birim_mizrakci > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_mizrakci;
}
    if (sayac_mizrakci == 0) {
        anlik_mizrakci_yer = mizrakci_yer;
    } else {

        anlik_mizrakci_yer = (Vector2){
            duzleme_ekleX + mizrakci_rastgele_x[sayac_mizrakci] * kare_boyut ,
            duzleme_ekleY + mizrakci_rastgele_y[sayac_mizrakci] * kare_boyut
        };
    }
    DrawTextureEx(mizrakci->resim, anlik_mizrakci_yer, 0.0, mizrakci->boyut, WHITE);
    canbari_olustur(anlik_mizrakci_yer, mizrakci->can, mizrakci->max_can);
    DrawText(mizrakci->isim, anlik_mizrakci_yer.x, anlik_mizrakci_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_mizrakci_yer.x, anlik_mizrakci_yer.y + 50, 10, BLACK);
    kalan_birim_mizrakci -= cizilecek_birimler;
    sayac_mizrakci++;
}



static int kusatma_rastgele_x[100];
static int kusatma_rastgele_y[100];
static bool pozisyon_atama_kusatma = false;
Vector2 kusatma_yer = (Vector2){
    duzleme_ekleX + kusatmamak->yer.x ,
    duzleme_ekleY + kusatmamak->yer.y
};
int kalan_birim_kusatma = kusatmamak->birim_sayisi;
if (!pozisyon_atama_kusatma) {
    for (int i = 0; i < 100; i++) {
        kusatma_rastgele_x[i] = rand() % 10 ;
        kusatma_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_kusatma = true;
}
int sayac_kusatma = 0;
Vector2 anlik_kusatma_yer;
while (kalan_birim_kusatma > 0) {
    int cizilecek_birimler;
if (kalan_birim_mizrakci > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_kusatma;
}
    if (sayac_kusatma == 0) {
        anlik_kusatma_yer = kusatma_yer;
    } else {
        anlik_kusatma_yer = (Vector2){
            duzleme_ekleX + kusatma_rastgele_x[sayac_kusatma] * kare_boyut ,
            duzleme_ekleY + kusatma_rastgele_y[sayac_kusatma] * kare_boyut
        };
    }
    DrawTextureEx(kusatmamak->resim, anlik_kusatma_yer, 0.0, kusatmamak->boyut, WHITE);
    canbari_olustur(anlik_kusatma_yer, kusatmamak->can, kusatmamak->max_can);
    DrawText(kusatmamak->isim, anlik_kusatma_yer.x, anlik_kusatma_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_kusatma_yer.x, anlik_kusatma_yer.y + 50, 10, BLACK);
    kalan_birim_kusatma -= cizilecek_birimler;
    sayac_kusatma++;
}



static int trol_rastgele_x[100];
static int trol_rastgele_y[100];
static bool pozisyon_atama_trol = false;
Vector2 trol_yer = (Vector2){
    duzleme_ekleX + trol->yer.x ,
    duzleme_ekleY + trol->yer.y
};
int kalan_birim_trol = trol->birim_sayisi;
if (!pozisyon_atama_trol) {
    for (int i = 0; i < 100; i++) {
        trol_rastgele_x[i] = rand() % 10 + 10;
        trol_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_trol = true;
}
int sayac_trol = 0;
Vector2 anlik_trol_yer;
while (kalan_birim_trol > 0) {
    int cizilecek_birimler;
if (kalan_birim_trol > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_trol;
}
    if (sayac_trol == 0) {
        anlik_trol_yer = trol_yer;
    } else {

        anlik_trol_yer = (Vector2){
            duzleme_ekleX + trol_rastgele_x[sayac_trol] * kare_boyut ,
            duzleme_ekleY + trol_rastgele_y[sayac_trol] * kare_boyut
        };
    }
    DrawTextureEx(trol->resim, anlik_trol_yer, 0.0, trol->boyut, WHITE);
    canbari_olustur(anlik_trol_yer, trol->can, trol->max_can);
    DrawText(trol->isim, anlik_trol_yer.x, anlik_trol_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_trol_yer.x, anlik_trol_yer.y + 50, 10, BLACK);
    kalan_birim_trol -= cizilecek_birimler;
    sayac_trol++;
}



static int varg_binicisi_rastgele_x[100];
static int varg_binicisi_rastgele_y[100];
static bool pozisyon_atama_varg = false;
Vector2 varg_binicisi_yer = (Vector2){
    duzleme_ekleX + varg_binicisi->yer.x ,
    duzleme_ekleY + varg_binicisi->yer.y
};
int kalan_birim_varg = varg_binicisi->birim_sayisi;
if (!pozisyon_atama_varg) {
    for (int i = 0; i < 100; i++) {
        varg_binicisi_rastgele_x[i] = rand() % 10+10;
        varg_binicisi_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_varg = true;
}
int sayac_varg = 0;
Vector2 anlik_varg_binicisi_yer;
while (kalan_birim_varg > 0) {
    int cizilecek_birimler;
if (kalan_birim_varg > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_varg;
}
    if (sayac_varg == 0) {
        anlik_varg_binicisi_yer = varg_binicisi_yer;
    } else {
        anlik_varg_binicisi_yer = (Vector2){
            duzleme_ekleX + varg_binicisi_rastgele_x[sayac_varg] * kare_boyut ,
            duzleme_ekleY + varg_binicisi_rastgele_y[sayac_varg] * kare_boyut
        };
    }
    DrawTextureEx(varg_binicisi->resim, anlik_varg_binicisi_yer, 0.0, varg_binicisi->boyut , WHITE);
    canbari_olustur(anlik_varg_binicisi_yer, varg_binicisi->can, varg_binicisi->max_can );
    DrawText(varg_binicisi->isim, anlik_varg_binicisi_yer.x, anlik_varg_binicisi_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_varg_binicisi_yer.x, anlik_varg_binicisi_yer.y + 50 , 10 , BLACK);
    kalan_birim_varg -= cizilecek_birimler;
    sayac_varg++;
}



static int suvari_rastgele_x[100];
static int suvari_rastgele_y[100];
static bool pozisyon_atama_suvari = false;
Vector2 suvari_yer = (Vector2){
    duzleme_ekleX + suvari->yer.x ,
    duzleme_ekleY + suvari->yer.y
};
int kalan_birim_suvari = suvari->birim_sayisi;
if (!pozisyon_atama_suvari) {
    for (int i = 0; i < 100; i++) {
        suvari_rastgele_x[i] = rand() % 10;
        suvari_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_suvari = true;
}
int sayac_suvari = 0;
Vector2 anlik_suvari_yer;
while (kalan_birim_suvari > 0) {
    int cizilecek_birimler;
if (kalan_birim_suvari > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_suvari;
}
    if (sayac_suvari == 0) {
        anlik_suvari_yer = suvari_yer;
    } else {
        anlik_suvari_yer = (Vector2){
            duzleme_ekleX + suvari_rastgele_x[sayac_suvari] * kare_boyut ,
            duzleme_ekleY + suvari_rastgele_y[sayac_suvari] * kare_boyut
        };
    }
    DrawTextureEx(suvari->resim, anlik_suvari_yer, 0.0, suvari->boyut , WHITE);
    canbari_olustur(anlik_suvari_yer, suvari->can, suvari->max_can);
    DrawText(suvari->isim, anlik_suvari_yer.x, anlik_suvari_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_suvari_yer.x, anlik_suvari_yer.y + 50 , 10 , BLACK);
    kalan_birim_suvari -= cizilecek_birimler;
    sayac_suvari++;
}



static int okcu_rastgele_x[100];
static int okcu_rastgele_y[100];
static bool pozisyon_atama_okcu = false;
Vector2 okcu_yer = (Vector2){
    duzleme_ekleX + okcu->yer.x ,
    duzleme_ekleY + okcu->yer.y
};
int kalan_birim_okcu = okcu->birim_sayisi;
if (!pozisyon_atama_okcu) {
    for (int i = 0; i < 100; i++) {
        okcu_rastgele_x[i] = rand() % 10;
        okcu_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_okcu = true;
}
int sayac_okcu = 0;
Vector2 anlik_okcu_yer;
while (kalan_birim_okcu > 0) {
    int cizilecek_birimler;
if (kalan_birim_okcu > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_okcu;
}
    if (sayac_okcu == 0) {
        anlik_okcu_yer = okcu_yer;
    } else {
        anlik_okcu_yer = (Vector2){
            duzleme_ekleX + okcu_rastgele_x[sayac_okcu] * kare_boyut ,
            duzleme_ekleY + okcu_rastgele_y[sayac_okcu] * kare_boyut
        };
    }
    DrawTextureEx(okcu->resim, anlik_okcu_yer, 0.0, okcu->boyut , WHITE);
    canbari_olustur(anlik_okcu_yer, okcu->can, okcu->max_can);
    DrawText(okcu->isim, anlik_okcu_yer.x, anlik_okcu_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_okcu_yer.x, anlik_okcu_yer.y + 50 , 10 , BLACK);
    kalan_birim_okcu -= cizilecek_birimler;
    sayac_okcu++;
}



static int piyade_rastgele_x[100];
static int piyade_rastgele_y[100];
static bool pozisyon_atama_piyade = false;
Vector2 piyade_yer = (Vector2){
    duzleme_ekleX + piyade->yer.x ,
    duzleme_ekleY + piyade->yer.y
};
int kalan_birim_piyade = piyade->birim_sayisi;
if (!pozisyon_atama_piyade) {
    for (int i = 0; i < 100; i++) {
        piyade_rastgele_x[i] = rand() % 10;
        piyade_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_piyade = true;
}
int sayac_piyade = 0;
Vector2 anlik_piyade_yer;
while (kalan_birim_piyade > 0) {
    int cizilecek_birimler;
if (kalan_birim_piyade> maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_piyade;
}
    if (sayac_piyade == 0) {
        anlik_piyade_yer = piyade_yer;
    } else {
        anlik_piyade_yer = (Vector2){
            duzleme_ekleX + piyade_rastgele_x[sayac_piyade] * kare_boyut ,
            duzleme_ekleY + piyade_rastgele_y[sayac_piyade] * kare_boyut
        };
    }
    DrawTextureEx(piyade->resim, anlik_piyade_yer, 0.0, piyade->boyut , WHITE);
    canbari_olustur(anlik_piyade_yer, piyade->can, piyade->max_can);
    DrawText(piyade->isim, anlik_piyade_yer.x, anlik_piyade_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_piyade_yer.x, anlik_piyade_yer.y + 50 , 10 , BLACK);
    kalan_birim_piyade -= cizilecek_birimler;
    sayac_piyade++;
}



static int ork_dovuscusu_rastgele_x[100];
static int ork_dovuscusu_rastgele_y[100];
static bool pozisyon_atama_ork_dovuscusu = false;
Vector2 ork_dovuscusu_yer = (Vector2){
    duzleme_ekleX + ork_dovuscusu->yer.x ,
    duzleme_ekleY + ork_dovuscusu->yer.y
};
int kalan_birim_ork_dovuscusu = ork_dovuscusu->birim_sayisi;
if (!pozisyon_atama_ork_dovuscusu) {
    for (int i = 0; i < 100; i++) {
        ork_dovuscusu_rastgele_x[i] = rand() % 10 + 10 ;
        ork_dovuscusu_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_ork_dovuscusu = true;
}
int sayac_ork_dovuscusu = 0;
Vector2 anlik_ork_dovuscusu_yer;
while (kalan_birim_ork_dovuscusu > 0) {
    int cizilecek_birimler;
if (kalan_birim_ork_dovuscusu > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_ork_dovuscusu;
};
    if (sayac_ork_dovuscusu == 0) {
        anlik_ork_dovuscusu_yer = ork_dovuscusu_yer;
    } else {
        anlik_ork_dovuscusu_yer = (Vector2){
            duzleme_ekleX + ork_dovuscusu_rastgele_x[sayac_ork_dovuscusu] * kare_boyut ,
            duzleme_ekleY + ork_dovuscusu_rastgele_y[sayac_ork_dovuscusu] * kare_boyut
        };
    }
    DrawTextureEx(ork_dovuscusu->resim, anlik_ork_dovuscusu_yer, 0.0, ork_dovuscusu->boyut, WHITE);
    canbari_olustur(anlik_ork_dovuscusu_yer, ork_dovuscusu->can, ork_dovuscusu->max_can);
    DrawText(ork_dovuscusu->isim, anlik_ork_dovuscusu_yer.x, anlik_ork_dovuscusu_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_ork_dovuscusu_yer.x, anlik_ork_dovuscusu_yer.y + 50, 10, BLACK);

    kalan_birim_ork_dovuscusu -= cizilecek_birimler;
    sayac_ork_dovuscusu++;
}

DrawText("LUTFEN SENARYONUN SONUNU GORMEK ICIN ENTERA BASINIZ!", 250, 950, 30, GREEN);
if(IsKeyPressed(KEY_ENTER)){
        if(ork_kazandi_mi){
                kusatmamak->birim_sayisi=0;
                piyade->birim_sayisi=0;
                okcu->birim_sayisi=0;
                suvari->birim_sayisi=0;
                BeginDrawing();
                senaryo_bitis_ciz(insan_deger,ork_deger,kusatmamak,piyade,okcu,trol,ork_dovuscusu,mizrakci,suvari,varg_binicisi,insan,ork,sarmasik,baslangic_arkaplan);
            EndDrawing();
        }
        else{
            trol->birim_sayisi=0;
            ork_dovuscusu->birim_sayisi=0;
            varg_binicisi->birim_sayisi=0;
            mizrakci->birim_sayisi=0;
            BeginDrawing();

                senaryo_bitis_ciz(insan_deger,ork_deger,kusatmamak,piyade,okcu,trol,ork_dovuscusu,mizrakci,suvari,varg_binicisi,insan,ork,sarmasik,baslangic_arkaplan);
            EndDrawing();
        }
}
EndDrawing();

    }
}
void senaryo_bitis_ciz(savas_hesapla *insan_deger,savas_hesapla *ork_deger,Birim* kusatmamak,Birim* piyade,Birim* okcu,Birim* trol,Birim* ork_dovuscusu,Birim* mizrakci,Birim* suvari,Birim* varg_binicisi,insan_savas* insan,ork_savas*ork,Texture2D sarmasik,Texture2D baslangic_arkaplan,bool ork_kazandimi){
       BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(baslangic_arkaplan, 0, 0, WHITE);
        DrawText("Savas Devam Ediyor...", 520, 492, 40, WHITE);
        EndDrawing();
    Sleep(3000);
BeginDrawing();
int maximum_birim = 100;
int ork_kazan;
if(ork_deger->saglik_total == 0){
    ork_kazan = 0;
}
else{
    ork_kazan=1;
}
while (!WindowShouldClose()) {
        int duzleme_ekleX = (ekran_genislik - (kare_boyut * 20)) / 2;
        int duzleme_ekleY = (ekran_uzunluk - (kare_boyut * 20)) / 2;
        BeginDrawing();
        ClearBackground(RAYWHITE);
                DrawTexture(baslangic_arkaplan, 0, 0, WHITE);
                DrawTexture(sarmasik, 170,70, WHITE);
                DrawTexture(sarmasik, 1040,70, WHITE);
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                int x = duzleme_ekleX + j * kare_boyut;
                int y = duzleme_ekleY + i * kare_boyut;
                Color boya;
                if (j < 10)
                {
                    boya = BROWN;
                }
                else
                {
                    boya = DARKGREEN;
                }
                DrawRectangle(x, y, kare_boyut, kare_boyut, boya);
                DrawRectangleLines(x, y, kare_boyut, kare_boyut, BLACK);
            }
        }



static int mizrakci_rastgele_x[100];
static int mizrakci_rastgele_y[100];
static bool pozisyon_atama_mizrakci = false;
Vector2 mizrakci_yer = (Vector2){
    duzleme_ekleX + mizrakci->yer.x ,
    duzleme_ekleY + mizrakci->yer.y
};
int kalan_birim_mizrakci;
    kalan_birim_mizrakci = 0;
if(ork_kazan){
 kalan_birim_mizrakci = ork->mizrakcilar_sayi;
}
if (!pozisyon_atama_mizrakci) {
    for (int i = 0; i < 100; i++) {
        mizrakci_rastgele_x[i] = rand() % 10 + 10 ;
        mizrakci_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_mizrakci = true;
}
int sayac_mizrakci = 0;
Vector2 anlik_mizrakci_yer;
while (kalan_birim_mizrakci > 0) {
    int cizilecek_birimler;
if (kalan_birim_mizrakci > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_mizrakci;
}
    if (sayac_mizrakci == 0) {
        anlik_mizrakci_yer = mizrakci_yer;
    } else {
        anlik_mizrakci_yer = (Vector2){
            duzleme_ekleX + mizrakci_rastgele_x[sayac_mizrakci] * kare_boyut ,
            duzleme_ekleY + mizrakci_rastgele_y[sayac_mizrakci] * kare_boyut
        };
    }
    DrawTextureEx(mizrakci->resim, anlik_mizrakci_yer, 0.0, mizrakci->boyut, WHITE);
    DrawText(mizrakci->isim, anlik_mizrakci_yer.x, anlik_mizrakci_yer.y + 40, 10, BLACK);
    if(ork->mizrakcilar_sayi == mizrakci->birim_sayisi && insan_deger->net_hasar<=0){
        canbari_olustur(anlik_mizrakci_yer,mizrakci->can,mizrakci->max_can);
    }
    else if(ork->mizrakcilar_sayi != mizrakci->birim_sayisi || insan_deger->net_hasar>0){
        canbari_olustur(anlik_mizrakci_yer, mizrakci->can*0.75, mizrakci->max_can);
    }
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_mizrakci_yer.x, anlik_mizrakci_yer.y + 50, 10, BLACK);
    kalan_birim_mizrakci -= cizilecek_birimler;
    sayac_mizrakci++;
}



static int kusatma_rastgele_x[100];
static int kusatma_rastgele_y[100];
static bool pozisyon_atama_kusatma = false;
Vector2 kusatma_yer = (Vector2){
    duzleme_ekleX + kusatmamak->yer.x ,
    duzleme_ekleY + kusatmamak->yer.y
};
int kalan_birim_kusatma ;
    kalan_birim_kusatma = insan->kusatmamak_sayi;
if(ork_kazan){
    kalan_birim_kusatma = 0;
}
if (!pozisyon_atama_kusatma) {
    for (int i = 0; i < 100; i++) {
        kusatma_rastgele_x[i] = rand() % 10 ;
        kusatma_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_kusatma = true;
}
int sayac_kusatma = 0;
Vector2 anlik_kusatma_yer;
while (kalan_birim_kusatma > 0) {
    int cizilecek_birimler;
if (kalan_birim_kusatma > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_kusatma;
}
    if (sayac_kusatma == 0) {
        anlik_kusatma_yer = kusatma_yer;
    } else {
        anlik_kusatma_yer = (Vector2){
            duzleme_ekleX + kusatma_rastgele_x[sayac_kusatma] * kare_boyut ,
            duzleme_ekleY + kusatma_rastgele_y[sayac_kusatma] * kare_boyut
        };
    }
    DrawTextureEx(kusatmamak->resim, anlik_kusatma_yer, 0.0, kusatmamak->boyut, WHITE);
    if(insan->kusatmamak_sayi == kusatmamak->birim_sayisi && ork_deger->net_hasar<=0){
        canbari_olustur(anlik_kusatma_yer,kusatmamak->can,kusatmamak->max_can);
    }
    else if(insan->kusatmamak_sayi != kusatmamak->birim_sayisi || ork_deger->net_hasar>0){
        canbari_olustur(anlik_kusatma_yer, kusatmamak->can*0.75, kusatmamak->max_can);
    }
    DrawText(kusatmamak->isim, anlik_kusatma_yer.x, anlik_kusatma_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_kusatma_yer.x, anlik_kusatma_yer.y + 50, 10, BLACK);
    kalan_birim_kusatma -= cizilecek_birimler;
    sayac_kusatma++;
}



static int trol_rastgele_x[100];
static int trol_rastgele_y[100];
static bool pozisyon_atama_trol = false;
Vector2 trol_yer = (Vector2){
    duzleme_ekleX + trol->yer.x ,
    duzleme_ekleY + trol->yer.y
};
int kalan_birim_trol;
kalan_birim_trol= 0;
if(ork_kazan){
    kalan_birim_trol = ork->troller_sayi;
}
if (!pozisyon_atama_trol) {
    for (int i = 0; i < 100; i++) {
        trol_rastgele_x[i] = rand() % 10 + 10;
        trol_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_trol = true;
}
int sayac_trol = 0;
Vector2 anlik_trol_yer;
while (kalan_birim_trol > 0) {
    int cizilecek_birimler;
if (kalan_birim_trol > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_trol;
}
    if (sayac_trol == 0) {
        anlik_trol_yer = trol_yer;
    } else {
        anlik_trol_yer = (Vector2){
            duzleme_ekleX + trol_rastgele_x[sayac_trol] * kare_boyut ,
            duzleme_ekleY + trol_rastgele_y[sayac_trol] * kare_boyut
        };
    }
    DrawTextureEx(trol->resim, anlik_trol_yer, 0.0, trol->boyut, WHITE);
     if(trol->birim_sayisi == ork->troller_sayi && insan_deger->net_hasar<=0){
        canbari_olustur(anlik_trol_yer,trol->can,trol->max_can);
    }
    else if(trol->birim_sayisi != ork->troller_sayi || insan_deger->net_hasar>0){
        canbari_olustur(anlik_trol_yer, trol->can*0.75, trol->max_can);
    }
    DrawText(trol->isim, anlik_trol_yer.x, anlik_trol_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_trol_yer.x, anlik_trol_yer.y + 50, 10, BLACK);
    kalan_birim_trol -= cizilecek_birimler;
    sayac_trol++;
}




static int varg_binicisi_rastgele_x[100];
static int varg_binicisi_rastgele_y[100];
static bool pozisyon_atama_varg = false;
Vector2 varg_binicisi_yer = (Vector2){
    duzleme_ekleX + varg_binicisi->yer.x ,
    duzleme_ekleY + varg_binicisi->yer.y
};
int kalan_birim_varg;
    kalan_birim_varg=0;
if(ork_kazan){
    kalan_birim_varg = ork->varg_binicileri_sayi;
}
if (!pozisyon_atama_varg) {
    for (int i = 0; i < 100; i++) {
        varg_binicisi_rastgele_x[i] = rand() % 10+10;
        varg_binicisi_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_varg = true;
}
int sayac_varg = 0;
Vector2 anlik_varg_binicisi_yer;
while (kalan_birim_varg > 0) {
    int cizilecek_birimler;
if (kalan_birim_varg > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_varg;
}
    if (sayac_varg == 0) {
        anlik_varg_binicisi_yer = varg_binicisi_yer;
    } else {
        anlik_varg_binicisi_yer = (Vector2){
            duzleme_ekleX + varg_binicisi_rastgele_x[sayac_varg] * kare_boyut ,
            duzleme_ekleY + varg_binicisi_rastgele_y[sayac_varg] * kare_boyut
        };
    }
    DrawTextureEx(varg_binicisi->resim, anlik_varg_binicisi_yer, 0.0, varg_binicisi->boyut , WHITE);
    if(ork->varg_binicileri_sayi == varg_binicisi->birim_sayisi && insan_deger->net_hasar<=0){
        canbari_olustur(anlik_varg_binicisi_yer,varg_binicisi->can,varg_binicisi->max_can);
    }
    else if(ork->varg_binicileri_sayi != varg_binicisi->birim_sayisi || insan_deger->net_hasar>0){
        canbari_olustur(anlik_varg_binicisi_yer, varg_binicisi->can*0.75, varg_binicisi->max_can);
    }
    DrawText(varg_binicisi->isim, anlik_varg_binicisi_yer.x, anlik_varg_binicisi_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_varg_binicisi_yer.x, anlik_varg_binicisi_yer.y + 50 , 10 , BLACK);

    kalan_birim_varg -= cizilecek_birimler;
    sayac_varg++;
}



static int suvari_rastgele_x[100];
static int suvari_rastgele_y[100];
static bool pozisyon_atama_suvari = false;
Vector2 suvari_yer = (Vector2){
    duzleme_ekleX + suvari->yer.x ,
    duzleme_ekleY + suvari->yer.y
};
int kalan_birim_suvari;
kalan_birim_suvari = insan->suvari_sayi;
if(ork_kazan){
     kalan_birim_suvari =0;
}
if (!pozisyon_atama_suvari) {
    for (int i = 0; i < 100; i++) {
        suvari_rastgele_x[i] = rand() % 10;
        suvari_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_suvari = true;
}
int sayac_suvari = 0;
Vector2 anlik_suvari_yer;
while (kalan_birim_suvari > 0) {
    int cizilecek_birimler = (kalan_birim_suvari > maximum_birim) ? maximum_birim : kalan_birim_suvari;
    if (sayac_suvari == 0) {
        anlik_suvari_yer = suvari_yer;
    } else {
        anlik_suvari_yer = (Vector2){
            duzleme_ekleX + suvari_rastgele_x[sayac_suvari] * kare_boyut ,
            duzleme_ekleY + suvari_rastgele_y[sayac_suvari] * kare_boyut
        };
    }
    DrawTextureEx(suvari->resim, anlik_suvari_yer, 0.0, suvari->boyut , WHITE);
   if(insan->suvari_sayi == suvari->birim_sayisi && ork_deger->net_hasar<=0){
        canbari_olustur(anlik_suvari_yer,suvari->can,suvari->max_can);
    }
    else if(insan->suvari_sayi != suvari->birim_sayisi || ork_deger->net_hasar>0){
        canbari_olustur(anlik_suvari_yer, suvari->can*0.75, suvari->max_can);
    }
    DrawText(suvari->isim, anlik_suvari_yer.x, anlik_suvari_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_suvari_yer.x, anlik_suvari_yer.y + 50 , 10 , BLACK);
    kalan_birim_suvari -= cizilecek_birimler;
    sayac_suvari++;
}




static int okcu_rastgele_x[100];
static int okcu_rastgele_y[100];
static bool pozisyon_atama_okcu = false;
Vector2 okcu_yer = (Vector2){
    duzleme_ekleX + okcu->yer.x ,
    duzleme_ekleY + okcu->yer.y
};
int kalan_birim_okcu;
     kalan_birim_okcu=insan->okcu_sayi;
if(ork_kazan){
     kalan_birim_okcu =0;
}
if (!pozisyon_atama_okcu) {
    for (int i = 0; i < 100; i++) {
        okcu_rastgele_x[i] = rand() % 10;
        okcu_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_okcu = true;
}
int sayac_okcu = 0;
Vector2 anlik_okcu_yer;
while (kalan_birim_okcu > 0) {
    int cizilecek_birimler;
if (kalan_birim_okcu > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_okcu;
}
    if (sayac_okcu == 0) {
        anlik_okcu_yer = okcu_yer;
    } else {
        anlik_okcu_yer = (Vector2){
            duzleme_ekleX + okcu_rastgele_x[sayac_okcu] * kare_boyut ,
            duzleme_ekleY + okcu_rastgele_y[sayac_okcu] * kare_boyut
        };
    }
    DrawTextureEx(okcu->resim, anlik_okcu_yer, 0.0, okcu->boyut , WHITE);
    if(insan->okcu_sayi == okcu->birim_sayisi && ork_deger->net_hasar<=0){
        canbari_olustur(anlik_okcu_yer,okcu->can,okcu->max_can);
    }
    else if(insan->okcu_sayi != okcu->birim_sayisi || ork_deger->net_hasar>0){
        canbari_olustur(anlik_okcu_yer, okcu->can*0.75, okcu->max_can);
    }
    DrawText(okcu->isim, anlik_okcu_yer.x, anlik_okcu_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_okcu_yer.x, anlik_okcu_yer.y + 50 , 10 , BLACK);
    kalan_birim_okcu -= cizilecek_birimler;
    sayac_okcu++;
}



static int piyade_rastgele_x[100];
static int piyade_rastgele_y[100];
static bool pozisyon_atama_piyade = false;
Vector2 piyade_yer = (Vector2){
    duzleme_ekleX + piyade->yer.x ,
    duzleme_ekleY + piyade->yer.y
};
int kalan_birim_piyade;
    kalan_birim_piyade=insan->piyade_sayi;
if(ork_kazan){
    kalan_birim_piyade =0;
}
if (!pozisyon_atama_piyade) {
    for (int i = 0; i < 100; i++) {
        piyade_rastgele_x[i] = rand() % 10;
        piyade_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_piyade = true;
}
int sayac_piyade = 0;
Vector2 anlik_piyade_yer;
while (kalan_birim_piyade > 0) {
    int cizilecek_birimler;
if (kalan_birim_piyade > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_piyade;
}
    if (sayac_piyade == 0) {
        anlik_piyade_yer = piyade_yer;
    } else {
        anlik_piyade_yer = (Vector2){
            duzleme_ekleX + piyade_rastgele_x[sayac_piyade] * kare_boyut ,
            duzleme_ekleY + piyade_rastgele_y[sayac_piyade] * kare_boyut
        };
    }

    DrawTextureEx(piyade->resim, anlik_piyade_yer, 0.0, piyade->boyut , WHITE);
    if(insan->piyade_sayi == piyade->birim_sayisi && ork_deger->net_hasar<=0){
        canbari_olustur(anlik_piyade_yer,piyade->can,piyade->max_can);
    }
    else if(insan->piyade_sayi != piyade->birim_sayisi || ork_deger->net_hasar>0){
        canbari_olustur(anlik_piyade_yer, piyade->can*0.75, piyade->max_can);

    }
    DrawText(piyade->isim, anlik_piyade_yer.x, anlik_piyade_yer.y + 40 , 10 , BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_piyade_yer.x, anlik_piyade_yer.y + 50 , 10 , BLACK);
    kalan_birim_piyade -= cizilecek_birimler;
    sayac_piyade++;
}




static int ork_dovuscusu_rastgele_x[100];
static int ork_dovuscusu_rastgele_y[100];
static bool pozisyon_atama_ork_dovuscusu = false;
Vector2 ork_dovuscusu_yer = (Vector2){
    duzleme_ekleX + ork_dovuscusu->yer.x ,
    duzleme_ekleY + ork_dovuscusu->yer.y
};
int kalan_birim_ork_dovuscusu;
    kalan_birim_ork_dovuscusu = 0;
if(ork_kazan){
    kalan_birim_ork_dovuscusu =ork->ork_dovusculeri_sayi;
}
if (!pozisyon_atama_ork_dovuscusu) {
    for (int i = 0; i < 100; i++) {
        ork_dovuscusu_rastgele_x[i] = rand() % 10 + 10 ;
        ork_dovuscusu_rastgele_y[i] = rand() % 20;
    }
    pozisyon_atama_ork_dovuscusu = true;
}
int sayac_ork_dovuscusu = 0;
Vector2 anlik_ork_dovuscusu_yer;
while (kalan_birim_ork_dovuscusu > 0) {
    int cizilecek_birimler;
if (kalan_birim_ork_dovuscusu > maximum_birim) {
    cizilecek_birimler = maximum_birim;
} else {
    cizilecek_birimler = kalan_birim_ork_dovuscusu;
}
    if (sayac_ork_dovuscusu == 0) {
        anlik_ork_dovuscusu_yer = ork_dovuscusu_yer;
    } else {
        anlik_ork_dovuscusu_yer = (Vector2){
            duzleme_ekleX + ork_dovuscusu_rastgele_x[sayac_ork_dovuscusu] * kare_boyut ,
            duzleme_ekleY + ork_dovuscusu_rastgele_y[sayac_ork_dovuscusu] * kare_boyut
        };
    }
    DrawTextureEx(ork_dovuscusu->resim, anlik_ork_dovuscusu_yer, 0.0, ork_dovuscusu->boyut, WHITE);
    if(ork->ork_dovusculeri_sayi == ork_dovuscusu->birim_sayisi&&insan_deger->net_hasar<=0){
        canbari_olustur(anlik_ork_dovuscusu_yer,ork_dovuscusu->can,ork_dovuscusu->max_can);
    }
    else if(ork->ork_dovusculeri_sayi != ork_dovuscusu->birim_sayisi||insan_deger->net_hasar>0){
        canbari_olustur(anlik_ork_dovuscusu_yer, ork_dovuscusu->can*0.75, ork_dovuscusu->max_can);
    }
    DrawText(ork_dovuscusu->isim, anlik_ork_dovuscusu_yer.x, anlik_ork_dovuscusu_yer.y + 40, 10, BLACK);
    DrawText(TextFormat("Birim Sayisi: %d", cizilecek_birimler), anlik_ork_dovuscusu_yer.x, anlik_ork_dovuscusu_yer.y + 50, 10, BLACK);
    kalan_birim_ork_dovuscusu -= cizilecek_birimler;
    sayac_ork_dovuscusu++;
    }
    if(insan_deger->saglik_total<=0){
            DrawText("ORKLAR KAZANDI!", 530, 450, 40, BLACK);

    }
    else if(ork_deger->saglik_total<=0){
        DrawText("INSANLAR KAZANDI!", 530, 450, 40, BLACK);
    }
    EndDrawing();
}
}

int main() {
    system("curl -o senaryo1.json https://yapbenzet.org.tr/1.json");
    system("curl -o senaryo2.json https://yapbenzet.org.tr/2.json");
    system("curl -o senaryo3.json https://yapbenzet.org.tr/3.json");
    system("curl -o senaryo4.json https://yapbenzet.org.tr/4.json");
    system("curl -o senaryo5.json https://yapbenzet.org.tr/5.json");
    system("curl -o senaryo6.json https://yapbenzet.org.tr/6.json");
    system("curl -o senaryo7.json https://yapbenzet.org.tr/7.json");
    system("curl -o senaryo8.json https://yapbenzet.org.tr/8.json");
    system("curl -o senaryo9.json https://yapbenzet.org.tr/9.json");
    system("curl -o senaryo10.json https://yapbenzet.org.tr/10.json");
    const char* creatures_file = "creatures.json";
    char* creatures = dosya_oku(creatures_file);
    if (creatures == NULL) {
        free(creatures);
    }
    const char* heroes_file = "heroes.json";
    char* heroes = dosya_oku(heroes_file);
    if (heroes == NULL) {
        free(heroes);
    }
    const char* research_file = "research.json";
    char* research = dosya_oku(research_file);
    if(research == NULL ){
       free(research);
    }
    const char* unittypes_file = "unit_types.json";
    char* unittypes = dosya_oku(unittypes_file);
    if(unittypes == NULL ){
       free(unittypes);
    }
    const char* senaryo_file1 = "senaryo1.json";
    char* senaryo1= dosya_oku(senaryo_file1);
    if(senaryo1==NULL){
        free(senaryo1);
    }
        const char* senaryo_file2= "senaryo2.json";
    char* senaryo2= dosya_oku(senaryo_file2);
    if(senaryo2==NULL){
        free(senaryo2);
    }
        const char* senaryo_file3= "senaryo3.json";
    char* senaryo3= dosya_oku(senaryo_file3);
    if(senaryo3==NULL){
        free(senaryo3);
    }
            const char* senaryo_file4= "senaryo4.json";
    char* senaryo4= dosya_oku(senaryo_file4);
    if(senaryo4==NULL){
        free(senaryo4);
    }
                const char* senaryo_file5= "senaryo5.json";
    char* senaryo5= dosya_oku(senaryo_file5);
    if(senaryo5==NULL){
        free(senaryo5);
    }
                    const char* senaryo_file6= "senaryo6.json";
    char* senaryo6= dosya_oku(senaryo_file6);
    if(senaryo6==NULL){
        free(senaryo6);
    }
                    const char* senaryo_file7= "senaryo7.json";
    char* senaryo7= dosya_oku(senaryo_file7);
    if(senaryo7==NULL){
        free(senaryo7);
    }
                    const char* senaryo_file8= "senaryo8.json";
    char* senaryo8= dosya_oku(senaryo_file8);
    if(senaryo8==NULL){
        free(senaryo8);
    }
                    const char* senaryo_file9= "senaryo9.json";
    char* senaryo9= dosya_oku(senaryo_file9);
    if(senaryo9==NULL){
        free(senaryo9);
    }
                    const char* senaryo_file10= "senaryo10.json";
    char* senaryo10= dosya_oku(senaryo_file10);
    if(senaryo10==NULL){
        free(senaryo10);
    }
    InitWindow(ekran_genislik, ekran_uzunluk, "Orkmania");
    SetTargetFPS(60);
    srand(time(NULL));
    Texture2D baslangic_arkaplan = LoadTexture("baslangic_arkaplan.png");
    Texture2D arkaplan = LoadTexture("background.png");
    Texture2D mizrakci_resim = LoadTexture("mizrakci.png");
    Texture2D kusatmamak_resim = LoadTexture("kusatma_makinesi.png");
    Texture2D suvari_resim = LoadTexture("suvari.png");
    Texture2D okcu_resim = LoadTexture("okcu.png");
    Texture2D trol_resim = LoadTexture("trol.png");
    Texture2D vargbinicisi_resim = LoadTexture("varg_binicisi.png");
    Texture2D piyade_resim = LoadTexture("piyade.png");
    Texture2D orkdovuscusu_resim = LoadTexture("ork_dovuscusu.png");
    Texture2D yazi = LoadTexture("orkmania_yazi.png");
    Texture2D sarmasik = LoadTexture("sarmasik.png");
    InitAudioDevice();
    Music music = LoadMusicStream("forest.mp3");
    PlayMusicStream(music);

    ustalik savunma,saldiri,elit_egitim,kusatma;
    bilgi_yukle_ustalik(research,&savunma,&saldiri,&elit_egitim,&kusatma);
    insan_hero alparslan, fatih, yavuz,metehan,tugrul;
    ork_hero goruk_vahsi, thruk_kemikkiran, vrog_kafakiran,ugar_zalim;

    hero_yukle(heroes, "Goruk_Vahsi","Alparslan", &goruk_vahsi , &alparslan);
    hero_yukle(heroes, "Thruk_Kemikkiran","Fatih_Sultan_Mehmet", &thruk_kemikkiran,&fatih);
    hero_yukle(heroes, "Vrog_Kafakiran","Yavuz_Sultan_Selim",&vrog_kafakiran,&yavuz);
    hero_yukle(heroes, "Ugar_Zalim","Tugrul_Bey",&ugar_zalim,&tugrul);
    hero_yukle(heroes,"Ugar_Zalim","Mete_Han",&ugar_zalim,&metehan);
    etkilenen_yukle_orkhero(&goruk_vahsi);


    insan_yaratik ejderha, agri_dagi_devleri, tepegoz, karakurt, samur;
    orc_yaratik kara_troll,golge_kurtlari,camur_devleri,ates_iblisi,buz_devleri,makrog_savas_beyi;
    yaratik_yukle(creatures,"Kara_Troll","Ejderha",&kara_troll,&ejderha);
    yaratik_yukle(creatures,"Golge_Kurtlari","Agri_Dagi_Devleri",&golge_kurtlari,&agri_dagi_devleri);
    yaratik_yukle(creatures,"Camur_Devleri","Tepegoz",&camur_devleri,&tepegoz);
    yaratik_yukle(creatures,"Ates_Iblisi","Karakurt",&ates_iblisi,&karakurt);
    yaratik_yukle(creatures,"Buz_Devleri","Samur",&buz_devleri,&samur);
    yaratik_yukle(creatures,"Makrog_Savas_Beyi","Samur",&makrog_savas_beyi,&samur);

    etkilenen_yukle_insanyaratik(&ejderha);
    etkilenen_yukle_insanyaratik(&agri_dagi_devleri);
    etkilenen_yukle_insanyaratik(&tepegoz);
    etkilenen_yukle_insanyaratik(&karakurt);
    etkilenen_yukle_insanyaratik(&samur);

    etkilenen_yukle_orkyaratik(&kara_troll);
    etkilenen_yukle_orkyaratik(&golge_kurtlari);
    etkilenen_yukle_orkyaratik(&camur_devleri);
    etkilenen_yukle_orkyaratik(&ates_iblisi);
    etkilenen_yukle_orkyaratik(&buz_devleri);
    etkilenen_yukle_orkyaratik(&makrog_savas_beyi);

    etkilenen_yukle_insanhero(&alparslan);
    etkilenen_yukle_insanhero(&fatih);
    etkilenen_yukle_insanhero(&metehan);
    etkilenen_yukle_insanhero(&yavuz);
    etkilenen_yukle_insanhero(&tugrul);

    etkilenen_yukle_orkhero(&goruk_vahsi);
    etkilenen_yukle_orkhero(&thruk_kemikkiran);
    etkilenen_yukle_orkhero(&vrog_kafakiran);
    etkilenen_yukle_orkhero(&ugar_zalim);

    etkilenen_yukle_research(&savunma.level_1);
    etkilenen_yukle_research(&savunma.level_2);
    etkilenen_yukle_research(&savunma.level_3);
    etkilenen_yukle_research(&saldiri.level_1);
    etkilenen_yukle_research(&saldiri.level_2);
    etkilenen_yukle_research(&saldiri.level_3);
    etkilenen_yukle_research(&elit_egitim.level_1);
    etkilenen_yukle_research(&elit_egitim.level_2);
    etkilenen_yukle_research(&elit_egitim.level_3);
    etkilenen_yukle_research(&kusatma.level_1);
    etkilenen_yukle_research(&kusatma.level_2);
    etkilenen_yukle_research(&kusatma.level_3);
    insan_imp insan;
    ork_legi ork;
    unit_yukle(unittypes,&insan,&ork);
    insan_savas insanz,insanz1,insanz2,insanz3,insanz4,insanz5,insanz6,insanz7,insanz8,insanz9;
    ork_savas orkz,orkz1,orkz2,orkz3,orkz4,orkz5,orkz6,orkz7,orkz8,orkz9;

    senaryocek_insan(senaryo1,&insanz);
    senaryocek_ork(senaryo1,&orkz);
    senaryocek_ork(senaryo2,&orkz1);
    senaryocek_insan(senaryo2,&insanz1);
    senaryocek_ork(senaryo3,&orkz2);
    senaryocek_insan(senaryo3,&insanz2);
    senaryocek_insan(senaryo4,&insanz3);
    senaryocek_ork(senaryo4,&orkz3);
    senaryocek_insan(senaryo5,&insanz4);
    senaryocek_ork(senaryo5,&orkz4);
    senaryocek_insan(senaryo6,&insanz5);
    senaryocek_ork(senaryo6,&orkz5);
    senaryocek_insan(senaryo7,&insanz6);
    senaryocek_ork(senaryo7,&orkz6);
    senaryocek_insan(senaryo8,&insanz7);
    senaryocek_ork(senaryo8,&orkz7);
    senaryocek_insan(senaryo9,&insanz8);
    senaryocek_ork(senaryo9,&orkz8);
    senaryocek_insan(senaryo10,&insanz9);
    senaryocek_ork(senaryo10,&orkz9);
    savas_hesapla insan_deger,ork_deger;
    int round_insan = 0;
    int round_ork = 0;
    int x;
    int maximum_birim = 100;
    int senaryo_sayi = 0;
    bool basladi_mi = false;
    int y=5;
   FILE* sonuc = fopen("savas_sim.txt","w");
   if(sonuc == NULL){
    printf("Dosya acilamadi!\n");
   }

    while (!WindowShouldClose()) {
        if (!basladi_mi) {
            BeginDrawing();
            ClearBackground(WHITE);

            DrawTexture(baslangic_arkaplan, 0, 0, WHITE);
            DrawTexture(yazi, 400, -50, WHITE);
            UpdateMusicStream(music);

            DrawText("OYUNA HOSGELDINIZ!", 530, 450, 40, BROWN);
            DrawText("1 ile 10 arasinda bir sayi girin ardindan entera basin!", 500, 500, 20, BROWN);
            DrawText(TextFormat("Girdiginiz Sayi: %d", senaryo_sayi), 525, 525, 20, BROWN);
            DrawText("*Arka plan seslerini kapatmak icin M tusuna basmasiniz yeterlidir.", 450, 560, 20, BROWN);
            DrawText("IBO", 1340, 850, 20, BROWN);

            if (IsKeyPressed(KEY_ONE)) senaryo_sayi = 1;
            if (IsKeyPressed(KEY_TWO)) senaryo_sayi = 2;
            if (IsKeyPressed(KEY_THREE)) senaryo_sayi = 3;
            if (IsKeyPressed(KEY_FOUR)) senaryo_sayi = 4;
            if (IsKeyPressed(KEY_FIVE)) senaryo_sayi = 5;
            if (IsKeyPressed(KEY_SIX)) senaryo_sayi = 6;
            if (IsKeyPressed(KEY_SEVEN)) senaryo_sayi = 7;
            if (IsKeyPressed(KEY_EIGHT)) senaryo_sayi = 8;
            if (IsKeyPressed(KEY_NINE)) senaryo_sayi = 9;
            if (IsKeyPressed(KEY_ZERO)) senaryo_sayi = 10;
            if (IsKeyPressed(KEY_ENTER) && senaryo_sayi >= 1 && senaryo_sayi <= 10) {
                basladi_mi = true;
            }
            if (IsKeyPressed(KEY_M)) {
            StopMusicStream(music);
        }
          EndDrawing();
        }
        else{
                switch(senaryo_sayi) {
    case 1:

    y=10;


    Birim kusatmamak = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz.kusatmamak_sayi};
    Birim piyade = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz.piyade_sayi};
    Birim suvari =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz.suvari_sayi};
    Birim okcu = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz.okcu_sayi};
    Birim trol = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz.troller_sayi};
    Birim varg_binicisi = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz.varg_binicileri_sayi };
    Birim mizrakci = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz.mizrakcilar_sayi };
    Birim ork_dovuscusu = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz,orkz,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz,orkz,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz,orkz,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz.piyade_sayi*insan.piyadeler.saldiri)+(insanz.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz.piyade_sayi*insan.piyadeler.savunma)+(insanz.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz.varg_binicileri_sayi)+(ork.troller.savunma*orkz.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz.varg_binicileri_sayi)+(ork.troller.saldiri*orkz.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
        bool orklar_kazandimi;
                do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz.piyade_sayi*insan.piyadeler.saldiri)+(insanz.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz.piyade_sayi*insan.piyadeler.savunma)+(insanz.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));

            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;

            }
            ork_birim_azalt(&orkz,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;

            }

            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz.varg_binicileri_sayi)+(ork.troller.saldiri*orkz.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz.varg_binicileri_sayi)+(ork.troller.savunma*orkz.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\ork net hasar:%f\n",ork_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_maskinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                   orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz,orkz,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz,orkz,&ork,&ork_deger);
        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);

        senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak,&piyade,&okcu,&trol,&ork_dovuscusu,&mizrakci,&suvari,&varg_binicisi,&insanz,&orkz,sarmasik,baslangic_arkaplan,orklar_kazandimi);


        if(insan_deger.saglik_total<=0){

            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){

            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 2:
        y=9;

    Birim kusatmamak1 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz1.kusatmamak_sayi};
    Birim piyade1 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz1.piyade_sayi};
    Birim suvari1 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz1.suvari_sayi};
    Birim okcu1 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz1.okcu_sayi};
    Birim trol1 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz1.troller_sayi};
    Birim varg_binicisi1 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz1.varg_binicileri_sayi };
    Birim mizrakci1 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz1.mizrakcilar_sayi };
    Birim ork_dovuscusu1 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz1.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz1,orkz1,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz1,orkz1,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz1,orkz1,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz1.piyade_sayi*insan.piyadeler.saldiri)+(insanz1.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz1.piyade_sayi*insan.piyadeler.savunma)+(insanz1.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz.suvari_sayi)+(insanz.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz1.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz1.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz1.varg_binicileri_sayi)+(ork.troller.savunma*orkz1.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz1.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz1.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz1.varg_binicileri_sayi)+(ork.troller.saldiri*orkz1.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);

                do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz1.piyade_sayi*insan.piyadeler.saldiri)+(insanz1.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz1.suvari_sayi)+(insanz1.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz1.piyade_sayi*insan.piyadeler.savunma)+(insanz1.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz1.suvari_sayi)+(insanz1.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz1,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz1,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz1,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz1,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);

            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                      orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz1.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz1.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz1.varg_binicileri_sayi)+(ork.troller.saldiri*orkz1.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz1.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz1.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz1.varg_binicileri_sayi)+(ork.troller.savunma*orkz1.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\ork net hasar:%f\n",ork_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz1,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz1,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz1,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz1,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){

             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                      orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz1,orkz1,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz1,orkz1,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
         senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak1,&piyade1,&okcu1,&trol1,&ork_dovuscusu1,&mizrakci1,&suvari1,&varg_binicisi1,&insanz1,&orkz1,sarmasik,baslangic_arkaplan,orklar_kazandimi);

        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 3:
        y=7;

    Birim kusatmamak2 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz2.kusatmamak_sayi};
    Birim piyade2 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz2.piyade_sayi};
    Birim suvari2 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz2.suvari_sayi};
    Birim okcu2 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz2.okcu_sayi};
    Birim trol2 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz2.troller_sayi};
    Birim varg_binicisi2 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz2.varg_binicileri_sayi };
    Birim mizrakci2 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz2.mizrakcilar_sayi };
    Birim ork_dovuscusu2 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz2.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz2,orkz2,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz2,orkz2,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz2,orkz2,&ork,&ork_deger);
         insan_deger.saldiri_total=(insanz2.piyade_sayi*insan.piyadeler.saldiri)+(insanz2.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz2.suvari_sayi)+(insanz2.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz2.piyade_sayi*insan.piyadeler.savunma)+(insanz2.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz2.suvari_sayi)+(insanz2.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz2.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz2.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz2.varg_binicileri_sayi)+(ork.troller.savunma*orkz2.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz2.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz2.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz2.varg_binicileri_sayi)+(ork.troller.saldiri*orkz2.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
        do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz2.piyade_sayi*insan.piyadeler.saldiri)+(insanz2.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz2.suvari_sayi)+(insanz2.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz2.piyade_sayi*insan.piyadeler.savunma)+(insanz2.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz2.suvari_sayi)+(insanz2.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz2,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz2,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz2,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz2,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);

            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz2.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz2.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz2.varg_binicileri_sayi)+(ork.troller.saldiri*orkz2.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz2.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz2.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz2.varg_binicileri_sayi)+(ork.troller.savunma*orkz2.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\ork net hasar:%f\n",ork_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz2,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz2,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz2,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz2,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz2,orkz2,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz2,orkz2,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
         senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak2,&piyade2,&okcu2,&trol2,&ork_dovuscusu2,&mizrakci2,&suvari2,&varg_binicisi2,&insanz2,&orkz2,sarmasik,baslangic_arkaplan,orklar_kazandimi);

        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 4:
        y=2;

    Birim kusatmamak3 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz3.kusatmamak_sayi};
    Birim piyade3 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz3.piyade_sayi};
    Birim suvari3 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz3.suvari_sayi};
    Birim okcu3 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz3.okcu_sayi};
    Birim trol3 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz3.troller_sayi};
    Birim varg_binicisi3 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz3.varg_binicileri_sayi };
    Birim mizrakci3 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz3.mizrakcilar_sayi };
    Birim ork_dovuscusu3 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz3.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz3,orkz3,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz3,orkz3,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz3,orkz3,&ork,&ork_deger);
         insan_deger.saldiri_total=(insanz3.piyade_sayi*insan.piyadeler.saldiri)+(insanz3.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz3.suvari_sayi)+(insanz3.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz3.piyade_sayi*insan.piyadeler.savunma)+(insanz3.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz3.suvari_sayi)+(insanz3.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz3.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz3.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz3.varg_binicileri_sayi)+(ork.troller.savunma*orkz3.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz3.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz3.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz3.varg_binicileri_sayi)+(ork.troller.saldiri*orkz3.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
                do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz3.piyade_sayi*insan.piyadeler.saldiri)+(insanz3.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz3.suvari_sayi)+(insanz3.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz3.piyade_sayi*insan.piyadeler.savunma)+(insanz3.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz3.suvari_sayi)+(insanz3.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz3,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz3,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz3,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz3,&ork,&ork_deger,insan_deger.net_hasar,"troller");
              fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz3.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz3.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz3.varg_binicileri_sayi)+(ork.troller.saldiri*orkz3.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz3.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz3.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz3.varg_binicileri_sayi)+(ork.troller.savunma*orkz3.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\ork net hasar:%f\n",ork_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz3,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz3,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz3,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz3,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){

             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz3,orkz3,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz3,orkz3,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
         senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak3,&piyade3,&okcu3,&trol3,&ork_dovuscusu3,&mizrakci3,&suvari3,&varg_binicisi3,&insanz3,&orkz3,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
                break;
    case 5:
        y=8;
    Birim kusatmamak4 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz4.kusatmamak_sayi};
    Birim piyade4 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz4.piyade_sayi};
    Birim suvari4 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz4.suvari_sayi};
    Birim okcu4 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz4.okcu_sayi};
    Birim trol4 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz4.troller_sayi};
    Birim varg_binicisi4 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz4.varg_binicileri_sayi };
    Birim mizrakci4 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz4.mizrakcilar_sayi };
    Birim ork_dovuscusu4 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz4.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz4,orkz4,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz4,orkz4,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz4,orkz,&ork,&ork_deger);
         insan_deger.saldiri_total=(insanz4.piyade_sayi*insan.piyadeler.saldiri)+(insanz4.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz4.suvari_sayi)+(insanz4.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz4.piyade_sayi*insan.piyadeler.savunma)+(insanz4.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz4.suvari_sayi)+(insanz4.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz4.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz4.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz4.varg_binicileri_sayi)+(ork.troller.savunma*orkz4.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz4.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz4.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz4.varg_binicileri_sayi)+(ork.troller.saldiri*orkz4.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
               do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz4.piyade_sayi*insan.piyadeler.saldiri)+(insanz4.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz4.suvari_sayi)+(insanz4.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz4.piyade_sayi*insan.piyadeler.savunma)+(insanz4.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz4.suvari_sayi)+(insanz4.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
           fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz4,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz4,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz4,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz4,&ork,&ork_deger,insan_deger.net_hasar,"troller");
             fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz4.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz4.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz4.varg_binicileri_sayi)+(ork.troller.saldiri*orkz4.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz4.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz4.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz4.varg_binicileri_sayi)+(ork.troller.savunma*orkz4.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\nork net hasar:%f\n",ork_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz4,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz4,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz4,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz4,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
             fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz4,orkz4,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz4,orkz4,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
         senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak4,&piyade4,&okcu4,&trol4,&ork_dovuscusu4,&mizrakci4,&suvari4,&varg_binicisi4,&insanz4,&orkz4,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 6:
        y=6;
      Birim kusatmamak5 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz5.kusatmamak_sayi};
      Birim piyade5 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz5.piyade_sayi};
      Birim suvari5 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz5.suvari_sayi};
      Birim okcu5 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz5.okcu_sayi};
      Birim trol5 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz5.troller_sayi};
      Birim varg_binicisi5 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz5.varg_binicileri_sayi };
      Birim mizrakci5 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz5.mizrakcilar_sayi };
      Birim ork_dovuscusu5 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz5.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz5,orkz5,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz5,orkz5,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz5,orkz5,&ork,&ork_deger);
       insan_deger.saldiri_total=(insanz5.piyade_sayi*insan.piyadeler.saldiri)+(insanz5.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz5.suvari_sayi)+(insanz5.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz5.piyade_sayi*insan.piyadeler.savunma)+(insanz5.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz5.suvari_sayi)+(insanz5.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz5.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz5.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz5.varg_binicileri_sayi)+(ork.troller.savunma*orkz5.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz5.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz5.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz5.varg_binicileri_sayi)+(ork.troller.saldiri*orkz5.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
                do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz5.piyade_sayi*insan.piyadeler.saldiri)+(insanz5.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz5.suvari_sayi)+(insanz5.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz5.piyade_sayi*insan.piyadeler.savunma)+(insanz5.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz5.suvari_sayi)+(insanz5.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz5,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz5,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz5,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz5,&ork,&ork_deger,insan_deger.net_hasar,"troller");
             fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz5.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz5.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz5.varg_binicileri_sayi)+(ork.troller.saldiri*orkz5.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz5.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz5.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz5.varg_binicileri_sayi)+(ork.troller.savunma*orkz5.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
             fprintf(sonuc,"\nork net hasar:%f\n",insan_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz5,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz5,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz5,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz5,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz5,orkz5,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz5,orkz5,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
        senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak5,&piyade5,&okcu5,&trol5,&ork_dovuscusu5,&mizrakci5,&suvari5,&varg_binicisi5,&insanz5,&orkz5,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
         break;
    case 7:
        y=16;
         Birim kusatmamak6 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz6.kusatmamak_sayi};
      Birim piyade6 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz6.piyade_sayi};
      Birim suvari6 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz6.suvari_sayi};
      Birim okcu6 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz6.okcu_sayi};
      Birim trol6 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz6.troller_sayi};
      Birim varg_binicisi6 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz6.varg_binicileri_sayi };
      Birim mizrakci6 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz6.mizrakcilar_sayi };
      Birim ork_dovuscusu6 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz6.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz6,orkz6,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz6,orkz6,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz6,orkz6,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz6.piyade_sayi*insan.piyadeler.saldiri)+(insanz6.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz6.suvari_sayi)+(insanz6.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz6.piyade_sayi*insan.piyadeler.savunma)+(insanz6.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz6.suvari_sayi)+(insanz6.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz6.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz6.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz6.varg_binicileri_sayi)+(ork.troller.savunma*orkz6.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz6.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz6.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz6.varg_binicileri_sayi)+(ork.troller.saldiri*orkz6.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
                               do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz6.piyade_sayi*insan.piyadeler.saldiri)+(insanz6.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz6.suvari_sayi)+(insanz6.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz6.piyade_sayi*insan.piyadeler.savunma)+(insanz6.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz6.suvari_sayi)+(insanz6.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz6,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz6,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz6,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz6,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz6.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz6.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz6.varg_binicileri_sayi)+(ork.troller.saldiri*orkz6.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz6.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz6.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz6.varg_binicileri_sayi)+(ork.troller.savunma*orkz6.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\nork net hasar:%f\n",insan_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz6,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz6,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz6,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz6,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz6,orkz6,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz6,orkz6,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
         senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak6,&piyade6,&okcu6,&trol6,&ork_dovuscusu6,&mizrakci6,&suvari6,&varg_binicisi6,&insanz6,&orkz6,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 8:
        y=1;

    Birim kusatmamak7 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz7.kusatmamak_sayi};
      Birim piyade7 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz7.piyade_sayi};
      Birim suvari7 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz7.suvari_sayi};
      Birim okcu7 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz7.okcu_sayi};
      Birim trol7 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz7.troller_sayi};
      Birim varg_binicisi7 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz7.varg_binicileri_sayi };
      Birim mizrakci7 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz7.mizrakcilar_sayi };
      Birim ork_dovuscusu7 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz7.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz7,orkz7,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz7,orkz7,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz7,orkz7,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz7.piyade_sayi*insan.piyadeler.saldiri)+(insanz7.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz7.suvari_sayi)+(insanz7.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz7.piyade_sayi*insan.piyadeler.savunma)+(insanz7.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz7.suvari_sayi)+(insanz7.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz7.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz7.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz7.varg_binicileri_sayi)+(ork.troller.savunma*orkz7.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz7.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz7.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz7.varg_binicileri_sayi)+(ork.troller.saldiri*orkz7.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);

                                              do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz7.piyade_sayi*insan.piyadeler.saldiri)+(insanz7.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz7.suvari_sayi)+(insanz7.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz7.piyade_sayi*insan.piyadeler.savunma)+(insanz7.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz7.suvari_sayi)+(insanz7.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz7,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz7,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz7,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz7,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz7.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz7.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz7.varg_binicileri_sayi)+(ork.troller.saldiri*orkz7.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz7.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz7.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz7.varg_binicileri_sayi)+(ork.troller.savunma*orkz7.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\nork net hasar:%f\n",insan_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz7,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz7,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz7,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz7,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
            if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz7,orkz6,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz7,orkz6,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
        senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak7,&piyade7,&okcu7,&trol7,&ork_dovuscusu7,&mizrakci7,&suvari7,&varg_binicisi7,&insanz7,&orkz7,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 9:
        y=25;
      Birim kusatmamak8 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz8.kusatmamak_sayi};
      Birim piyade8 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz8.piyade_sayi};
      Birim suvari8 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz8.suvari_sayi};
      Birim okcu8 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz8.okcu_sayi};
      Birim trol8 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz8.troller_sayi};
      Birim varg_binicisi8 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz8.varg_binicileri_sayi };
      Birim mizrakci8 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz8.mizrakcilar_sayi };
      Birim ork_dovuscusu8 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz8.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz8,orkz8,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz8,orkz8,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz8,orkz8,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz8.piyade_sayi*insan.piyadeler.saldiri)+(insanz8.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz8.suvari_sayi)+(insanz8.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz8.piyade_sayi*insan.piyadeler.savunma)+(insanz8.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz8.suvari_sayi)+(insanz8.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz8.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz8.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz8.varg_binicileri_sayi)+(ork.troller.savunma*orkz8.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz8.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz8.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz8.varg_binicileri_sayi)+(ork.troller.saldiri*orkz8.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
                                   do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz8.piyade_sayi*insan.piyadeler.saldiri)+(insanz8.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz8.suvari_sayi)+(insanz8.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz8.piyade_sayi*insan.piyadeler.savunma)+(insanz8.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz8.suvari_sayi)+(insanz8.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz8,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz8,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz8,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz8,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
            if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
        //                                      SIMDI ORKLAR

                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz8.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz8.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz8.varg_binicileri_sayi)+(ork.troller.saldiri*orkz8.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz8.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz8.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz8.varg_binicileri_sayi)+(ork.troller.savunma*orkz8.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\nork net hasar:%f\n",insan_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz8,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz8,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz8,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz8,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
             if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }

        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz8,orkz8,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz8,orkz8,&ork,&ork_deger);

        }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
           senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak8,&piyade8,&okcu8,&trol8,&ork_dovuscusu8,&mizrakci8,&suvari8,&varg_binicisi8,&insanz8,&orkz8,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
        break;
    case 10:
        y=15;
         Birim kusatmamak9 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, kusatmamak_resim, 0.8, insan.kusatma_makineleri.saglik,insan.kusatma_makineleri.saglik, "Kusatma Makinesi",insanz9.kusatmamak_sayi};
      Birim piyade9 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, piyade_resim, 0.8,insan.piyadeler.saglik, insan.piyadeler.saglik, "Piyade",insanz9.piyade_sayi};
      Birim suvari9 =  { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, suvari_resim, 0.8, insan.suvariler.saglik,insan.suvariler.saglik, "Suvari",insanz9.suvari_sayi};
      Birim okcu9 = { (Vector2){ (rand() % 10) * kare_boyut, (rand() % 20) * kare_boyut }, okcu_resim, 0.8,insan.okcular.saglik,insan.okcular.saglik,"Okcu",insanz9.okcu_sayi};
      Birim trol9 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, trol_resim, 0.7, ork.troller.saglik ,ork.troller.saglik , "Trol",orkz9.troller_sayi};
      Birim varg_binicisi9 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, vargbinicisi_resim, 0.8, ork.varg_binicileri.saglik, ork.varg_binicileri.saglik, "Varg Binicisi",orkz9.varg_binicileri_sayi };
      Birim mizrakci9 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, mizrakci_resim, 0.8, ork.mizrakcilar.saglik, ork.mizrakcilar.saglik, "Mizrakci",orkz9.mizrakcilar_sayi };
      Birim ork_dovuscusu9 = { (Vector2){ (rand() % 10 + 10) * kare_boyut, (rand() % 20) * kare_boyut }, orkdovuscusu_resim, 0.8, ork.ork_dovusculeri.saglik, ork.ork_dovusculeri.saglik, "Ork Dovuscusu",orkz9.ork_dovusculeri_sayi};

        kritik_sans_yukle(research,heroes,creatures,&insan,insanz9,orkz9,&ork,&ork_deger,&insan_deger);
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz9,orkz9,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz9,orkz9,&ork,&ork_deger);
        insan_deger.saldiri_total=(insanz9.piyade_sayi*insan.piyadeler.saldiri)+(insanz9.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz9.suvari_sayi)+(insanz9.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
        insan_deger.savunma_total=(insanz9.piyade_sayi*insan.piyadeler.savunma)+(insanz9.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz9.suvari_sayi)+(insanz9.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
        ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz9.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz9.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz9.varg_binicileri_sayi)+(ork.troller.savunma*orkz9.troller_sayi);
        ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz9.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz9.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz9.varg_binicileri_sayi)+(ork.troller.saldiri*orkz9.troller_sayi);
        fprintf(sonuc,"---------------------------ORK LEGI VE INSANLAR ARASINDA SURECEK CETIN SAVAS BASLADI!---------------------------\n\n");
        fprintf(sonuc,"BIRLIKLERIN BASLANGIC BILGILERI:\n");
        fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f\nork saldiri total :%f\nork savunma total:%f\n",insan_deger.saldiri_total,insan_deger.savunma_total,ork_deger.saldiri_total,ork_deger.savunma_total);
        fprintf(sonuc,"insan toplam saglik : %f\nork toplam saglik :%f\n\n",insan_deger.saglik_total,ork_deger.saglik_total);
                                               do{
            if(((round_insan+round_ork) % 5) == 0 && round_insan!=0){
            // saldiri
            insan.kusatma_makineleri.saldiri=insan.kusatma_makineleri.saldiri*0.9;
            insan.okcular.saldiri=insan.okcular.saldiri*0.9;
            insan.piyadeler.saldiri=insan.piyadeler.saldiri*0.9;
            insan.suvariler.saldiri=insan.suvariler.saldiri*0.9;
            // savunma
            insan.kusatma_makineleri.savunma=insan.kusatma_makineleri.savunma*0.9;
            insan.okcular.savunma=insan.okcular.savunma*0.9;
            insan.piyadeler.savunma=insan.piyadeler.savunma*0.9;
            insan.suvariler.savunma=insan.suvariler.savunma*0.9;
            }
            insan_deger.saldiri_total=(insanz9.piyade_sayi*insan.piyadeler.saldiri)+(insanz9.okcu_sayi*insan.okcular.saldiri)+(insan.suvariler.saldiri*insanz9.suvari_sayi)+(insanz9.kusatmamak_sayi*insan.kusatma_makineleri.saldiri);
            insan_deger.savunma_total=(insanz9.piyade_sayi*insan.piyadeler.savunma)+(insanz9.okcu_sayi*insan.okcular.savunma)+(insan.suvariler.savunma*insanz9.suvari_sayi)+(insanz9.kusatmamak_sayi*insan.kusatma_makineleri.savunma);
            round_insan++;
            if((round_insan %(100/insan.piyadeler.kritik_sans))==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.piyadeler.saldiri*0.5);
            }
            if(round_insan %(100/insan.okcular.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.okcular.saldiri*0.5);
            }
            if(round_insan %(100/insan.suvariler.kritik_sans)==0){
            insan_deger.saldiri_total=insan_deger.saldiri_total+(insan.suvariler.saldiri*0.5);
            }
            insan_deger.net_hasar=insan_deger.saldiri_total*(1-(ork_deger.savunma_total/insan_deger.saldiri_total));
            fprintf(sonuc,"\n -----INSANLAR VURUYOR -----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"insan saldiri total %f\ninsan savunma total : %f",insan_deger.saldiri_total,insan_deger.savunma_total);
            if(insan_deger.net_hasar <= 0 ){
                insan_deger.net_hasar = 0;
            }
            ork_birim_azalt(&orkz9,&ork,&ork_deger,insan_deger.net_hasar,"ork_dovusculeri");
            ork_birim_azalt(&orkz9,&ork,&ork_deger,insan_deger.net_hasar,"varg_binicileri");
            ork_birim_azalt(&orkz9,&ork,&ork_deger,insan_deger.net_hasar,"mizrakcilar");
            ork_birim_azalt(&orkz9,&ork,&ork_deger,insan_deger.net_hasar,"troller");
            fprintf(sonuc,"\ninsan net hasar:%f\n",insan_deger.net_hasar);
            ork_deger.saglik_total=ork_deger.saglik_total-insan_deger.net_hasar;
             if(ork_deger.saglik_total<0){
             ork_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(ork_deger.saglik_total<=0){
                    orklar_kazandimi=false;
                break;
            }
                        if(((round_insan+round_ork) % 5) == 0 && round_ork != 0) {
            // saldiri
            ork.mizrakcilar.saldiri=ork.mizrakcilar.saldiri*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.saldiri*0.9;
            ork.troller.saldiri=ork.troller.saldiri*0.9;
            ork.varg_binicileri.saldiri=ork.varg_binicileri.saldiri*0.9;
            // savunma
            ork.mizrakcilar.savunma=ork.mizrakcilar.savunma*0.9;
            ork.ork_dovusculeri.saldiri=ork.ork_dovusculeri.savunma*0.9;
            ork.troller.savunma=ork.troller.savunma*0.9;
            ork.varg_binicileri.savunma=ork.varg_binicileri.savunma*0.9;
            }
            ork_deger.saldiri_total=(ork.ork_dovusculeri.saldiri*orkz9.ork_dovusculeri_sayi)+(ork.mizrakcilar.saldiri*orkz9.mizrakcilar_sayi)+(ork.varg_binicileri.saldiri*orkz9.varg_binicileri_sayi)+(ork.troller.saldiri*orkz9.troller_sayi);
            ork_deger.savunma_total=(ork.ork_dovusculeri.savunma*orkz9.ork_dovusculeri_sayi)+(ork.mizrakcilar.savunma*orkz9.mizrakcilar_sayi)+(ork.varg_binicileri.savunma*orkz9.varg_binicileri_sayi)+(ork.troller.savunma*orkz9.troller_sayi);
            round_ork++;
            if(round_ork %(100/ork.mizrakcilar.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.mizrakcilar.saldiri*0.5);
            }
            if(round_ork %(100/ork.ork_dovusculeri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.ork_dovusculeri.saldiri*0.5);
            }
            if(round_ork %(100/ork.troller.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.troller.saldiri*0.5);
            }
            if(round_ork %(100/ork.varg_binicileri.kritik_sans)==0){
            ork_deger.saldiri_total=ork_deger.saldiri_total+(ork.varg_binicileri.saldiri*0.5);
            }
            fprintf(sonuc,"\n-----ORKLAR VURUYOR-----\n");
            fprintf(sonuc,"\n --------%d. ROUND--------\n",round_insan+round_ork);
            fprintf(sonuc,"ork saldiri total %f\nork savunma total : %f\n",ork_deger.saldiri_total,ork_deger.savunma_total);
            ork_deger.net_hasar=ork_deger.saldiri_total*(1-(insan_deger.savunma_total/ork_deger.saldiri_total));
            if(ork_deger.net_hasar < 0){
                ork_deger.net_hasar = 0;
            }
            fprintf(sonuc,"\nork net hasar:%f\n",insan_deger.net_hasar);
            insan_deger.saglik_total=insan_deger.saglik_total-ork_deger.net_hasar;
            insan_birim_azalt(&insanz9,&insan,&insan_deger,ork_deger.net_hasar,"piyadeler");
            insan_birim_azalt(&insanz9,&insan,&insan_deger,ork_deger.net_hasar,"okcular");
            insan_birim_azalt(&insanz9,&insan,&insan_deger,ork_deger.net_hasar,"suvariler");
            insan_birim_azalt(&insanz9,&insan,&insan_deger,ork_deger.net_hasar,"kusatma_makinesi");
             if(insan_deger.saglik_total<0){
             insan_deger.saglik_total=0;
            }
            fprintf(sonuc,"insan round : %d\ninsan saglik : %f \nork round : %d\nork saglik :%f\n\n",round_insan,insan_deger.saglik_total,round_ork,ork_deger.saglik_total);
            if(insan_deger.saglik_total<=0){
                    orklar_kazandimi=true;
                break;
            }
        savas_hesapla_deger_insan(research,heroes,creatures,&insan,insanz9,orkz9,&ork,&insan_deger);
        savas_hesapla_deger_ork(research,heroes,creatures,&insan,insanz9,orkz9,&ork,&ork_deger);

 }while(insan_deger.saglik_total>0||ork_deger.saglik_total>0);
        senaryoyu_ciz(&insan_deger,&ork_deger,&kusatmamak9,&piyade9,&okcu9,&trol9,&ork_dovuscusu9,&mizrakci9,&suvari9,&varg_binicisi9,&insanz9,&orkz9,sarmasik,baslangic_arkaplan,orklar_kazandimi);
        if(insan_deger.saglik_total<=0){
            fprintf(sonuc,"\n\n------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI ORKLAR OLUYOR!");
        }
        else if(ork_deger.saglik_total<=0){
            fprintf(sonuc,"------------------- KAZANAN ------------------\n");
            fprintf(sonuc,"\n CETIN SAVASIN GALIBI INSANLAR OLUYOR!");
        }
          break;
    default:
        printf("Gecersiz secim!\n");
        break;
}
EndDrawing();
        }
    }

    UnloadTexture(arkaplan);
    UnloadTexture(mizrakci_resim);
    UnloadTexture(kusatmamak_resim);
    UnloadTexture(suvari_resim);
    UnloadTexture(okcu_resim);
    UnloadTexture(trol_resim);
    UnloadTexture(vargbinicisi_resim);
    UnloadTexture(piyade_resim);
    UnloadTexture(orkdovuscusu_resim);
    CloseWindow();
    free(senaryo10);
    free(senaryo9);
    free(senaryo8);
    free(senaryo7);
    free(senaryo6);
    free(senaryo5);
    free(senaryo4);
    free(senaryo3);
    free(senaryo2);
    free(senaryo1);
    free(unittypes);
    free(creatures);
    free(heroes);
    free(research);
    return 0;
}

