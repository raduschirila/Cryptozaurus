/*
Created: 06/2014
Author: Radu Chirila
Title: Hybrid encryption algorithm
*/

#include<iostream>
#include<cstring>
#include<cstdio>
#include<windows.h>
#include<ctime>
#include<cstdlib>
#define clrscr system("cls")
#define pause system("pause")
using namespace std;
struct nod{
	char ch;
	int nr;
	nod *urm;
}*v,*sf,*c,*pr,*a,*primul,*v_copie,*sf_copie;
int copie;
char s[4096],mesaj[1024];
void setclipboard(char *output)
{
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}
void create(char c,nod *&vf,nod*&sfarsit)//eclipsez variabilele globale
{
	if(!vf)
	{
		vf=new nod;
		vf->ch=c;
		vf->urm=NULL;
		sfarsit=vf;
	}
	else
	{
		nod *p;
		p=new nod;
		p->ch=c;
		p->urm=NULL;
		sfarsit->urm=p;
		sfarsit=p;
	}
}
void delete_list()
{
    nod *p,*r;
    p=v;
    while(p!=NULL)
    {
        r=p;
        p=p->urm;
        delete r;
    }
    delete p;
}
int numara()
{
    nod *u;
    int n=0;
    u=v;
    while(u->ch!='~')
    {
        n++;
        u=u->urm;
    }
    return n;
}
bool ok()
{
    nod *m;
    m=v;
    while(m->urm!=NULL)
    {
        if(m->ch=='~') return false;
        m=m->urm;
    }
    return true;
}
void create_key1(int i)
{
    c=new nod;
    c->urm=pr;
    c->nr=i;
    pr=c;
}
void create_key2(int i)
{
    a=new nod;
    a->urm=primul;
    a->nr=i;
    primul=a;
}
int get_key1(int &seed,int &douamii)
{
    printf("CHEIA 1: ");
    int k;
    scanf("%d %d %d",&seed,&k,&douamii);
    return k;
}
int get_key2(int &k,int&dmi)
{
    printf("CHEIA 2: ");
    int seed;
    scanf("%d %d %d",&k,&seed,&dmi);
    return seed;
}
inline int anul()
{
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    return aTime->tm_year+1900;
}
bool validare_an(int an_dat)
{
    int an_curent=anul();
    return(an_dat==an_curent?true:false);
}
void af(nod *prim)
{
	nod *p;
	p=prim;
	while(p!=NULL)
	{
		cout<<p->ch;
		p=p->urm;
	}
}
void afnr(nod *in)
{
    nod *i;
    i=in;
    while(i!=NULL)
    {
        printf("%d ",i->nr);
        i=i->urm;
    }
}
void init()
{
	system("cls");
	printf("ATENTIE! MESAJUL TREBUIE SA SE TERMINE CU ACEST CARACTER: ~(TILDA).DACA PROCESUL DE CRIPTARE DUREAZA MAI MULT DE 5 SECUNDE, INCHIDETI PROGRAMUL SI MAI INCERCATI ODATA. NU SCRIETI MESAJE FOARTE LUNGI!\n");
	printf("Mesaj: ");
	char o;
	do{
            scanf("%c",&o);
            create(o,v,sf);
            create(o,v_copie,sf_copie);
	}while(o!='~');
	nod *p,*r=v_copie;
	p=v;
	v=v->urm;
	v_copie=v_copie->urm;
	delete p;delete r;
}
void crypt_1(int k)//cifrul lui Cezar modificat
{
    srand(time(NULL));
    int seed=rand()%11+1;
    copie=seed;
    create_key1(anul());
    create_key1(k);
    create_key1(seed);

    nod *p;
    p=v;
    while(p->ch!='~')
    {
        p->ch+=seed;
        if(p->ch=='~'){
                p->ch++;
        }
        if(seed >= 256){
                seed=copie;
        }
        else seed+=k;

        p=p->urm;
    }
}
void decrypt_1(int seed,int k)
{
        nod *p;
        p=v;
        copie=seed;
        while(p->ch!='~')
        {
            p->ch-=seed;
            if(p->ch==127) p->ch--;
            if(seed >= 256) seed=copie;
            else seed+=k;
            p=p->urm;
        }
}
void crypt_2(int seed, int k)
{
    int x=2;
    nod *r;
    r=v;
    while(r->ch!='~')
    {
        r->ch^=seed;
        if(x%2==0)
        seed+=k;
        else seed-=k;
        r=r->urm;
    }
}
/*void strings()
{
    strcpy(s,"MESAJ: ");
    int i=0;
    nod *p;
    p=v;
    while(p!=NULL)
    {
        mesaj[i]=(char)p->ch;
        p=p->urm;i++;
    }
    strcat(s,mesaj);
    setclipboard(s);
}*/
void strt()
{
	int opt;
	int s1,s2,k1,k2,seed,k,a,year1,year2;
	printf("1.Criptare\n2.Decriptare\n3.Extra\n4.Iesire\nOptiune: ");
	scanf("%d",&opt);
	system("cls");
	switch(opt)
	{
		case 1:
		    	init();
				aici:
				srand(time(NULL));
				seed=rand()%11+1;
				k=rand()%10+1;

				create_key2(anul());
                create_key2(seed);
                create_key2(k);

				crypt_1(rand()%11+1);
				crypt_2(seed,k);

				if(not ok())//mai baga o fisa
                {
                    delete_list();
                    //incarca lista salvata
                    v=v_copie;
                    goto aici;
                }
				//strings();*/
				printf("\nMesaj criptat: ");
				af(v);
				printf("\n");
				printf("\nCHEIE 1: ");
				afnr(pr);
				printf("\n");
				printf("CHEIE 2: ");
				afnr(primul);
				printf("\n\n");
				printf("________________________________________________________________________________");
				printf("PENTRU A COPIA MESAJUL, APASATI CLICK-DREAPTA IN INTERIORUL FERESTREI, CLICK PE MARK, SELECTATI CEEA CE VRETI SA COPIATI (MESAJ SI CHEI) SI DATI PASTE IN FISIERUL TEXT DESCHIS...SALVATI CU ENCODING UNICODE SI INCHIDETI\n");
				system("pause");
				freopen("cod_criptat.txt","w",stdout);
				system("notepad.exe cod_criptat.txt");
				fclose(stdout);
				system("del cod_criptat.txt");
				printf("\n");
				break;
		case 2:
                init();
                k1=get_key1(s1,year1);
                if(not validare_an(year1))
                {
                    printf("CHEIE INVALIDA..\nINCHIDERE");
                    return;
                }
                s2=get_key2(k2,year2);
                if(not validare_an(year2))
                {
                    printf("CHEIE INVALIDA..\nINCHIDERE");
                    return;
                }
                crypt_2(s2,k2);
                decrypt_1(s1,k1);
                af(v);
				break;
        case 3:
                printf("\t\t\tProiect realizat de Chirila Radu\n\t\t\t\tVersiune BETA 0.1\n \n \n Limbaj programare: C++\n Numar de linii de cod: aprox 320\n Procedee de programare folosite: Alocare dinamica, Recursivitate, Siruri de caractere, Operatii pe biti\n");
                printf(" Mod de criptare a mesajelor: textul introdus este supus unor modificari cu ajutorul cifrului lui Cezar hibridizat(de mine), apoi se aplica criptarea Xor(operatie pe biti) cu proprietati modificate.\n\n");
                printf("\n\n Pentru a copia codul in mod corect, trebuie sa dati click-dreapta oriunde in fereastra, sa selectati optiunea 'Mark', apoi sa apasati tasta Enter\n\n\n");
                printf(" ________\n");
                printf(" Atentie! Cheile generate de program sunt necesare in procesul de decriptare,NU  le pierdeti!!\n\n\n");
                system("pause");
                system("cls");
                strt();
        case 4:
                return;
                break;
		default:
				system("cls");
				strt();
				break;
	}
}
int main()
{
    system("color 0a");
	strt();
	printf("\n");
	printf("\n");
	system("pause");
	return 0;
}
