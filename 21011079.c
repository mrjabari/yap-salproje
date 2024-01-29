#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <conio.h> 
#define ITERASYON 400//otomatik mod iterasyonu
#define GAMETIME 100

//otomatik mod recursive algoritma kullanarak sadece cýkýsa ulasýyor.

struct Player {
    char *name;//dosyada da her bir kullanýcý bilgisi bu sýrayla tutuluor, isim, soyisim,nickname,sifre.
    char *surname;
    char *nickname;
    char *password;
    int score;
};
void recursiveAuto(char** gameBoard ,int initialRow, int initialColumn, int row, int column,char temp,char*stack,int* numberOfElements,int*flag,char***buffer,int *iterasyon);
void autoPlay();
int mainMenu(FILE* myFile);
void signin(FILE* myFile);
int login(FILE* myFile,struct Player*player);
void readFromFile(char**gameBoard, FILE *gameFile);
void drawBoard(char** gameBoard, int row , int column);
int game();
int manuelPlay(char** gameBoard,int row , int column);
char *takeTheMove();
void bubbleSort(struct Player* players, int scoreSayisi);
void skorGoruntule();
//void readTheLine(char *a,FILE* myFile);
int main() 
{ 
struct Player temp;
FILE* myFile ;
myFile = fopen("myFile","ab+");
if(myFile== NULL){
	printf("an error has occured\n");
}


mainMenu(myFile);


fclose(myFile);
return 0; 
} 

int mainMenu(FILE* myFile){//bu fonksiyon ile ana menuyu yazdýrdým
	int welcome;
	int enter;

	struct Player* player;
	player=(struct Player*)calloc(1,sizeof(struct Player));
	player->nickname = (char*)calloc(50,sizeof(char));

	do{
	printf("\n\nOyuna hosgeldiniz.\n1: Log in\n2: Sign in\n3: Oyun skorlarini goruntule\n4: Exit\n");
	scanf("%d",&welcome);
	int i;
	switch(welcome){
		case(1):
           enter = login(myFile,player);
           if(enter == 1 ){
           	//oyuna yonlendir.
           		FILE* scoreFile;
	        scoreFile = fopen("scoreFile.txt","ab");
           	int flag =1;
            do{
            	
            	 printf("\nOtomatik mod icin 1 ' e, manuel mod icin 2 ye, menuye donmek icin 3 'e  basiniz\n");
            scanf("%d",&i);
			 if(i==1){
			 	autoPlay();
			 }else if(i==2){
			 	int puan = game();//burada oyundan elde edilen puaný dosyaya aktarmak için puan döndürdüm
			 	fprintf(scoreFile,"%d %s\n",puan,(player->nickname));  
			 }else if(i==3){
			     flag =0;
			 }else{
			 		printf("Gecersiz giris yaptiniz.\n");
			 }
			}while(flag==1);
           
            fclose(scoreFile);
		   }else if(enter ==0){
		   	   sleep(1);
		   	   system("cls");
			   welcome = 10;//boylece tekrar kullaniciya menu sunulacak.
		   }
			break;
	
		case(2):
			signin(myFile);
			break;
		
		case(3):
			skorGoruntule();
			break;
		case(4):

			break;
        default:
    	    printf("Gecersiz giris yaptiniz\n");
        	break;
	}

}while(welcome!=4);

}

void skorGoruntule(){//burada dosyada varolan skorlarý dosyadan struct dizisine aktarýp onlarý sýralayarak ekrana ilk 5 ini yazdýrdým.
	FILE* scoreFile=fopen("scoreFile.txt","rb");
	struct Player* players = (struct Player* )calloc(20,sizeof(struct Player));
	system("cls");
	int scoreSayisi=0;
	int i ,j;
	int tempPuan;
	char* tempNick;
	tempNick=(char*)calloc(50,sizeof(char));
	
	while(!feof(scoreFile)){
		
		fscanf(scoreFile,"%d %s\n",&tempPuan,tempNick);
		players[scoreSayisi].score=tempPuan;
		players[scoreSayisi].nickname=(char*)calloc(50,sizeof(char));
		strcpy(players[scoreSayisi].nickname,tempNick);
		scoreSayisi++;
		
	}
	
	bubbleSort(players, scoreSayisi);
	printf("En yuksek skorlar : \n");
	for (i=0;i<5;i++){
		
		if(players[i].nickname!=NULL)
		printf("%d . : %d %s\n",i+1,players[i].score,players[i].nickname);
	} 
	fclose(scoreFile);
}
void bubbleSort(struct Player* players, int scoreSayisi) {//dosyadan alýnan skorlarý bubble sort ile sýraladým
    int i, j;
    for (i = 0; i < scoreSayisi-1; i++) {
        for (j = 0; j < scoreSayisi-i-1; j++) {
            if (players[j].score < players[j+1].score) {
                struct Player temp = players[j];
                players[j] = players[j+1];
                players[j+1] = temp;
            }
        }
    }
}

int game(){//burada oyunu manuelplaya yonlendirdim.

	FILE* gameFile;
	char* fileName;
	fileName=(char*)calloc(50,sizeof(char));
	int row,column,i;
	printf("\nDosya ismi giriniz.");
	scanf("%s",fileName);
//	fileName="6x10.txt";
	gameFile = fopen(fileName,"r");
	
	printf("\nOynanacak oyun tahtasinin sirasiyla satir ve sutun degerlerini giriniz.");
	scanf("%d",&row);
	scanf("%d",&column);
	// row =6;column=10;
	
	char** gameBoard;
	gameBoard = (char**)calloc(row,sizeof(char*));
	for(i=0;i<row;i++){
		gameBoard[i]= (char*)calloc(column,sizeof(char));
	}
	
	readFromFile(gameBoard,gameFile);
	drawBoard(gameBoard,row,column);
  int puan =   manuelPlay(gameBoard,row , column);
  
  return puan;
}

int manuelPlay(char** gameBoard,int row , int column){//burada manuel play kýsmýný kodladým.
	
	int i ,j ,flag = 0;//dongu degiskenleri.
	
	int karsitElektron=0;
	int karsitProton=0;
	int proton=0;
	int elektron=0;//bunlar toplanan elemanlarýn sayýsýný tutuyor.
	
	int exit ;
	char temp;
	char* userMove;
	int initialRow,initialColumn;
	char* stack;
	int numberOfElements=0;
	int timeLeft;
	int passedTime;
	int puan;
	stack = (char*)malloc(50*sizeof(char));//elde edilen birimleri burada saklýyorum.
	
	for(i=0;i<row;i++){
		for(j=0;j<column;j++){
			if(gameBoard[i][j]=='G'){
				temp = gameBoard[i][j];
				gameBoard[i][j]='X';
				initialRow= i ;
				initialColumn =j;
				j=column+1;
				i=row+1;
			}
		}
	}//burada giriþ noktasýný tespit edip x i oraya yerlestiriyorum.
	time_t startTime = time(NULL);
	while ( flag == 0){
    time_t simdikiZaman = time(NULL);
    
    passedTime = difftime(simdikiZaman, startTime);
    timeLeft = GAMETIME - passedTime;
    if(timeLeft>0){
    	//sure bitmediyse her hareket sonrasý sureyi yazdýrýyor.
    printf("\nKalan sure: %d saniye\n", timeLeft);
	userMove = takeTheMove();
	}
    //buradan sonraki kýsýmda up- down- left -right hareketleri icin birbirine cok benzer 4 kod blogu bulunuyor, ilk kod blogunda yorum satýrlarý bulunuyor,
	// diðerleri icin ayný durumlar gecerli
	if(userMove == "up"){
	    if(initialRow!=0&&gameBoard[initialRow-1][initialColumn]!='1'){
	    	
	    	if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]=0;
			}
			
			
			if(gameBoard[initialRow-1][initialColumn]=='K'){
				printf("\n\n\nKaradeligi gormezden gelmenin bedeli agirdir. Seni uzayin derinliklerine gonderiyorum!");
				flag =1;
				exit=1;//karadelige girersek oyun sonlanmasý icin bu degiskenleri kullandým.
			}else if(gameBoard[initialRow-1][initialColumn]=='G'){
			    temp='G';
			    gameBoard[initialRow-1][initialColumn]='X';
			}else if(gameBoard[initialRow-1][initialColumn]=='C'){
				
				printf("Cikisa ulastiniz!");
				
				flag=1;
			}else if(gameBoard[initialRow-1][initialColumn]==0){
				 gameBoard[initialRow-1][initialColumn]='X';
			}else{
				stack[numberOfElements]=gameBoard[initialRow-1][initialColumn];
				numberOfElements++;
				gameBoard[initialRow-1][initialColumn]='X';//ilerledigimiz konumda eger bir parcacik var ise onu stack adli diziye ekleyip element sayýsýný arttýrdým
			}
			
	    	initialRow=initialRow-1;
		}
	    
	}else if(userMove == "left"){
	//	printf("sol ok yonune bastiniz.\n");
		if(initialColumn!=0&&gameBoard[initialRow][initialColumn-1]!='1'){
	    	
	    	if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]=0;
			}
			
			
			if(gameBoard[initialRow][initialColumn-1]=='K'){
				printf("\n\n\n\nKaradeligi gormezden gelmenin bedeli agirdir. Seni uzayin derinliklerine gonderiyorum!\n");
				flag =1;
				exit=1;
			}else if(gameBoard[initialRow][initialColumn-1]=='G'){
			    temp='G';
			    gameBoard[initialRow][initialColumn-1]='X';
			}else if(gameBoard[initialRow][initialColumn-1]=='C'){
				
				printf("Cikisa ulastiniz!");
				
				flag=1;
			}else if(gameBoard[initialRow][initialColumn-1]==0){
				 gameBoard[initialRow][initialColumn-1]='X';
			}else{
				stack[numberOfElements]=gameBoard[initialRow][initialColumn-1];
				numberOfElements++;
				gameBoard[initialRow][initialColumn-1]='X';
			}
			
	    	initialColumn = initialColumn-1;
		}
	}else if (userMove=="right"){
	//	printf("sag ok yonune bastiniz.\n");
			if(initialColumn!=column-1&&gameBoard[initialRow][initialColumn+1]!='1'){
	    	
	    	if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]=0;
			}
			
			
			if(gameBoard[initialRow][initialColumn+1]=='K'){
				printf("\n\n\n\nKaradeligi gormezden gelmenin bedeli agirdir. Seni uzayin derinliklerine gonderiyorum!\n");
				flag =1;
				exit=1;
			}else if(gameBoard[initialRow][initialColumn+1]=='G'){
			    temp='G';
			    gameBoard[initialRow][initialColumn+1]='X';
			}else if(gameBoard[initialRow][initialColumn+1]=='C'){
				
				printf("Cikisa ulastiniz!");
				
				flag=1;
			}else if(gameBoard[initialRow][initialColumn+1]==0){
				 gameBoard[initialRow][initialColumn+1]='X';
			}else{
				stack[numberOfElements]=gameBoard[initialRow][initialColumn+1];
				numberOfElements++;
				gameBoard[initialRow][initialColumn+1]='X';
			}
			
	    	initialColumn = initialColumn+1;
		}
	}else if (userMove=="down"){
		//printf("asagi ok yonune bastiniz.\n");
		  if(initialRow!=row-1&&gameBoard[initialRow+1][initialColumn]!='1'){
	    	
	    	if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]=0;
			}
			
			
			if(gameBoard[initialRow+1][initialColumn]=='K'){
				printf("\n\n\nKaradeligi gormezden gelmenin bedeli agirdir. Seni uzayin derinliklerine gonderiyorum!");
				flag =1;
				exit = 1;
			}else if(gameBoard[initialRow+1][initialColumn]=='G'){
			    temp='G';
			    gameBoard[initialRow+1][initialColumn]='X';
			}else if(gameBoard[initialRow+1][initialColumn]=='C'){
				
				printf("Cikisa ulastiniz!");
				
				flag=1;
			}else if(gameBoard[initialRow+1][initialColumn]==0){
				 gameBoard[initialRow+1][initialColumn]='X';
			}else{
				stack[numberOfElements]=gameBoard[initialRow+1][initialColumn];
				numberOfElements++;
				gameBoard[initialRow+1][initialColumn]='X';
			}
			
	    	initialRow=initialRow+1;
		}
	}else if (userMove=="exit"){//eger kullanýcý kendi istegiyle cikis yaparsa donguden cikiliyor.
		printf("\nCikis yaptiniz.\n");
		exit =1;
		flag = 1;
	}

if(exit!=1){
	system("cls");
	drawBoard(gameBoard,row,column);
}
	
		printf("\n\n");
		for(i=0;(stack[i]<122&&stack[i]>65);i++){//her adýmdan sonra elde edilen parcaciklar yazdiriliyor.
			switch(stack[i]){
				case 'P':
					printf("p+ ");
				break;
				
				case 'p':
				    printf("p- ");	
				break;
				
				case 'E':
					printf("e+ ");
				break;
				
				case 'e':
				    printf("e- ");	
				break;
			}
    	}
    	
    	  if (timeLeft <= 0) {//sürenin bitmesi durumunda yine donguden cýkýlýyor ve puan elde edilmiyorç
            printf("\nOyunu kaybettiniz! Zaman doldu.\n");
            flag=1;
            exit = 1;
        }
	}
	
	
	if(exit!=1){ //eger cýkýsa ulasýlýrsa bu blok çalýþýyor. elde edilen parçacýklara göre puan yazdýrýlýyor.
	
		printf("\n\n");
		for(i=0;(stack[i]<122&&stack[i]>65);i++){
			switch(stack[i]){
				case 'P':
					printf("p+ ");
					proton++;
				break;
				
				case 'p':
					karsitProton++;
				    printf("p- ");	
				break;
				
				case 'E':
					karsitElektron++;
					printf("e+ ");
				break;
				
				case 'e':
					elektron++;
				    printf("e- ");	
				break;
			}
	}
	karsitElektron=karsitElektron-elektron;
	karsitProton=karsitProton-proton;
		int karsitMadde = karsitElektron>karsitProton?karsitProton:karsitElektron;
		printf("\n%d adet karsit madde uretildi\n",karsitMadde);
		
		puan = karsitMadde*50+timeLeft*1;
		printf("\nBitirme sureniz : %d saniye \nPuaniniz ( karsitMaddeSayisi*50+kalanSure*1) = %d\n",passedTime, puan);
	}

return puan;
}

char *takeTheMove(){//bu fonksiyon ile kullanýcýdan yön tuþlarý vasýtasýyla hareket aldým.
	char movement ;
	char* movementvalue;
	printf("\nGitmek istediginiz yone yon tuslari araciligiyla basiniz. Cikis icin 'Esc' tusuna basiniz.\n");
	
	movement = getch();
	 if (movement == 27) {  // ESC tuþu kontrolü
	 movementvalue="exit";
        return movementvalue;
    } else if (movement == -32) {  // Ok tuþlarý için kontrol
        movement = getch();  // Ýkinci karakteri al

        switch (movement) {
            case 72:
                movementvalue="up";
                break;
            case 75:
                movementvalue="left";
                break;
            case 77:
                movementvalue="right";
                break;
            case 80:
                movementvalue="down";
                break;
        }
    }

	
	return movementvalue ;
}
void drawBoard(char** gameBoard, int row , int column){//oyun alanýný yazdýrmak icin bu fonksiyonu kullandým.
    int i,j,k;
    for(i=0;i<row;i++){
        printf("\n");
        for (k=0;k<column;k++)
            printf("-------");
        printf("\n");
        for(j=0;j<column;j++)
            if (gameBoard[i][j]!=0&&gameBoard[i][j]!='2')
                printf("  %c   |",gameBoard[i][j]);
            else
                 printf("      |",gameBoard[i][j]);
    }
   
}

void readFromFile(char**gameBoard, FILE *gameFile){//oyun alanýný dosyadan burada okudum.
	int i,j;
	char temp;

    while(!feof(gameFile)){
        fscanf(gameFile,"%d %d %c\n",&i,&j,&temp);  
		gameBoard[i][j]=temp; 
    }  
  
}
int login(FILE* myFile,struct Player*player){//kullanýcý giriþi fonksiyonu
	int enter;
	
	char *userNick=(char*)calloc(50,sizeof(char));
	char *tempNick=(char*)calloc(50,sizeof(char));
	char *tempPassword=(char*)calloc(50,sizeof(char));
	char *userPassword=(char*)calloc(50,sizeof(char)); 
	int bigflag;
	int passwordflag;
	do{
	printf("Nickname giriniz.\n");
	scanf("%s",userNick);
	
	int flag=0;
	 bigflag=0;
	 fseek(myFile, 0, SEEK_SET);
	 
	 fgets(tempNick,50,myFile);fgets(tempNick,50,myFile);//burada dosyada ilk basta 2 satir atlamak icin böyle bi sey yaptým.
	 
	while(flag==0&&!feof(myFile)){//burada nickleri tek tek gezerek karsýlastýrdým.
		
	fgets(tempNick,50,myFile);
	strtok(tempNick, "\n");
	if(!strcmp(userNick,tempNick)){
		flag=1;
	}

	if(flag!=1){
		fgets(tempNick,50,myFile);fgets(tempNick,50,myFile);fgets(tempNick,50,myFile);//burada da 3 satýr atladým. cunku dosyada her bir 4 satýr bir structý ifade ediyor
		                                                                              //ornegin ilk kisinin nicki 2.satýrda ise 2. kisinin nicki 5. satýrda olacak. nicklere erismek icin boyle bir yol izledim.
	}
	 
	
	}
	if(flag==1){
		bigflag=1;
		passwordflag=1;
		strcpy(player->nickname,userNick);
		
	}else{
		printf("Boyle bir kullanici bulunmamaktadir. Lutfen tekrar deneyiniz.\n");//eðer o nicke dosyadan erisilemez ise bu blok calýsýyor.
		enter =0;
		bigflag=1;
		passwordflag=0;
	}
	
}while(bigflag==0);


if(passwordflag==1){
	printf("Lutfen sifrenizi giriniz.\n");//ayný islemler sifre icin yapýlýyor.
	scanf("%s",userPassword);
		fgets(tempPassword,50,myFile);
		
		strtok(tempPassword, "\n");
	if(!strcmp(tempPassword,userPassword)){
		enter =1;
		printf("\nGiris yapilmistir!\n");
		
	}else{	
	enter =0;
    printf("sifre yanlistir.\n");
}
}		
free(userNick);
free(tempNick);
free(userPassword);
free(tempPassword);
return enter ;//burada dogru giris yapýlýp yapýlmadýgýýný menuye dönduruyorum. buna göre oyun baslatýlýoyr ya da yeniden giris isteniyor.
}

void signin(FILE* myFile){//oyuncu bilgileri kayýt fonksiyonu
char *tempNick;
int flag;
int bigflag;
struct Player player;
tempNick=(char*)calloc(50,sizeof(char));

player.name=(char*)calloc(50,sizeof(char));
player.nickname=(char*)calloc(50,sizeof(char));
player.password=(char*)calloc(50,sizeof(char));
player.surname=	(char*)calloc(50,sizeof(char));
	printf("Isminizi giriniz.\n");
	scanf("%s",player.name);
	
	printf("Soyisminizi giriniz.\n");
	scanf("%s",player.surname);
do{
	printf("Nickname giriniz.\n");
	scanf("%s",player.nickname);
	
	flag=0;
	bigflag=0;
	 fseek(myFile, 0, SEEK_SET);
	 
	 fgets(tempNick,50,myFile);//ayný login fonksiyonundaki gibi 2 satýr atlamak adýna bu islemleri yaptým.
	 fgets(tempNick,50,myFile);
	 
	while(flag==0&&!feof(myFile)){//burada daha once boyle bir nick alýnýp alýnmadýgýný kontrol ettim.
		fgets(tempNick,50,myFile);
		strtok(tempNick, "\n");
	if(!strcmp(player.nickname,tempNick)){
		flag=1;
		
		printf("Bu kullanici adi zaten kullanimda, lutfen baska bir kullanici adi belirleyiniz.\n");
	}
if(flag!=1){

	 fgets(tempNick,50,myFile);//ayný login fonksiyonundaki gibi 3 satýr atlamak adýna bu islemleri yaptým.
	 fgets(tempNick,50,myFile);
	 fgets(tempNick,50,myFile);
	 

}

	}
		if(flag!=1){
		bigflag=1;
	}
	
}while(bigflag==0);
	
	printf("Sifrenizi giriniz.\n");
	scanf("%s",player.password);
	fputs(player.name,myFile);
	fputc('\n',myFile);
	fputs(player.surname,myFile);
	fputc('\n',myFile);
	fputs(player.nickname,myFile);
	fputc('\n',myFile);
	fputs(player.password,myFile);
	fputc('\n',myFile);

	printf("Kaydiniz yapilmistir!\n");
	
}
void autoPlay(){//otomatik oyuna giriþ fonksiyonu -- oyun alaný bilgileri alýnýp recursive fonksiyona yonlendiriliyor.
	FILE* gameFile;
	char* fileName;
	int row,column,i;
	int karsitElektron=0;
	int karsitProton=0;
	int proton=0;
	int elektron=0;
	fileName=(char*)calloc(30,sizeof(char));
	printf("Dosya ismi giriniz.");
	scanf("%s",fileName);
//	fileName="6x10.txt";
	gameFile = fopen(fileName,"rb");
	printf("Oynanacak oyun tahtasinin sirasiyla satir ve sutun degerlerini giriniz.");
	scanf("%d",&row);
	scanf("%d",&column);
	// row =6;column=10;
	
	char** gameBoard;
	gameBoard = (char**)calloc(row+1,sizeof(char*));
	for(i=0;i<row;i++){
		gameBoard[i]= (char*)calloc(column+1,sizeof(char));
	}
	
	readFromFile(gameBoard,gameFile);
	int j ,initialRow,initialColumn;
	int a ,b,c;
	int* numberOfElements=&a;
	int* iterasyon =&c;
	int* flag=&b;
	c=0;//a-b-c degiskenlerini pointerlara atamak icin oylesine olusturdum.
	a=0;
	b=0;
	char*stack=(char*)malloc(50*(sizeof(char)));
	char temp;
		for(i=0;i<row;i++){//oyun alanýndaki giriþi tespit amaclý burayý kullandým
		for(j=0;j<column;j++){
			if(gameBoard[i][j]=='G'){
				temp = gameBoard[i][j];
				gameBoard[i][j]='X';
				initialRow= i ;
				initialColumn =j;
				j=column+1;
				i=row+1;
			}
		}
	}
	 char ***buffer = (char ***)malloc(ITERASYON * sizeof(char **));//bu diziyi oyun alanýnýn her adýmdaki halini tutmak icin actým.

    for (i = 0; i < ITERASYON; ++i) {
        buffer[i] = (char **)malloc(row * sizeof(char *));
        for (j = 0; j < row; ++j) {
            buffer[i][j] = (char *)malloc(column * sizeof(char));
        }
    }
	
	recursiveAuto(gameBoard ,initialRow, initialColumn,row,column,temp,stack,numberOfElements,flag,buffer,iterasyon);
     
     	printf("Toplanan elementler : ");
		 	for(i=0;(stack[i]<122&&stack[i]>65);i++){
			switch(stack[i]){
				case 'P':
					printf("p+ ");
					proton++;
				break;
				
				case 'p':
					karsitProton++;
				    printf("p- ");	
				break;
				
				case 'E':
					karsitElektron++;
					printf("e+ ");
				break;
				
				case 'e':
					elektron++;
				    printf("e- ");	
				break;
			}
	}
	
		karsitElektron=karsitElektron-elektron;
	karsitProton=karsitProton-proton;
		int karsitMadde = karsitElektron>karsitProton?karsitProton:karsitElektron;
		printf("\n%d adet karsit madde uretildi\n",karsitMadde);
    printf("\n Toplam Iterasyon sayisi : %d\n",*iterasyon);
}

void recursiveAuto(char** gameBoard ,int initialRow, int initialColumn, int row, int column,char temp,char*stack,int* numberOfElements,int*flag,char***buffer,int *iterasyon){

	int i,j;
	if(*numberOfElements!=0&&*flag==0){//her adýmda toplanan elementler yazdýrýlýyor.
			printf("Toplanan elementler : ");
		 	for(i=0;(stack[i]<122&&stack[i]>65);i++){
			switch(stack[i]){
				case 'P':
					printf("p+ ");
				break;
				
				case 'p':
				    printf("p- ");	
				break;
				
				case 'E':
					printf("e+ ");
				break;
				
				case 'e':
				    printf("e- ");	
				break;
			}
	}
	}
if(*flag==0){

for(i=0;i<row;i++){//her adýmda buffer dizisine oyun alaný kaydediliyor.
	for(j=0;j<column;j++){
		buffer[*iterasyon][i][j]=gameBoard[i][j];
	}
}
	drawBoard(gameBoard,row,column);
	printf("\n\n\n");
	printf("\n\n\n");
	
	if((initialRow!=(row-1) )&&(gameBoard[initialRow+1][initialColumn]!='1')&&(gameBoard[initialRow+1][initialColumn]!='K')&&(gameBoard[initialRow+1][initialColumn]!='2')){
	
	
	
	
		if((gameBoard[initialRow+1][initialColumn]=='C')){//cýkýsa ulasýrsa recursive i bitiriyor.
			*flag=1;
			gameBoard[initialRow+1][initialColumn]='X';
		}
		    if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){//oyun alanýnda hareket ettigim yere 2 sayýsý koyuyorum boylece gittigim yere bir daha gidip dongude sýkýþýp kalmýyorum.
				gameBoard[initialRow][initialColumn]='2';
			}
	  
		
		if(gameBoard[initialRow+1][initialColumn]!=0&&gameBoard[initialRow+1][initialColumn]!='G'){
			for(i=0;i<ITERASYON;i++){//eger bir parcacik elde edersem buffer dizisinde bütün adýmlara ulasýp element elde ettigim noktayý sýfýr yapýyorum.
			                         //aksi taktirde recursive o yoldan geri donunce o yolda alýnan elementler tekrar ekrana yazdýrýlýyor.
				buffer[i][initialRow+1][initialColumn]=0;
			}
			
	  	stack[*numberOfElements]=gameBoard[initialRow+1][initialColumn];// o parcacýgý stack dizisine kaydettim.
	  	*numberOfElements=*numberOfElements+1;
	  }
	  temp=0;
	  gameBoard[initialRow+1][initialColumn]='X';
	  if(gameBoard[initialRow+1][initialColumn]=='G'){
	  	temp='G';
	  }
	  *iterasyon=*iterasyon+1;
	recursiveAuto(gameBoard ,initialRow+1, initialColumn,row,column,temp,stack,numberOfElements,flag,buffer,iterasyon);
	  //recursvie cagri
		//asagi
	}
	//yukardaki islemlerin aynilarýný asagýdaki 3 yon için de tekrarladým.
	if(initialColumn!=column-1&&gameBoard[initialRow][initialColumn+1]!='1'&&(gameBoard[initialRow][initialColumn+1]!='K')&&(gameBoard[initialRow][initialColumn+1]!='2')){

	
		if((gameBoard[initialRow][initialColumn+1]=='C')){
			*flag=1;
			gameBoard[initialRow][initialColumn+1]='X';
		}
		
		 if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]='2';
			}
	  
		
		if(gameBoard[initialRow][initialColumn+1]!=0&&gameBoard[initialRow][initialColumn+1]!='G'){
			
				for(i=0;i<ITERASYON;i++){
				buffer[i][initialRow][initialColumn+1]=0;
			}
	  	stack[*numberOfElements]=gameBoard[initialRow][initialColumn+1];
	  	*numberOfElements=*numberOfElements+1;
	  }
	  temp=0;
	  gameBoard[initialRow][initialColumn+1]='X';
	  if(gameBoard[initialRow][initialColumn+1]=='G'){
	  	temp='G';
	  }
	   *iterasyon=*iterasyon+1;
	  recursiveAuto(gameBoard ,initialRow, initialColumn+1,row,column,temp,stack,numberOfElements,flag,buffer,iterasyon);
	  //recursvie cagri
			//sag
		}
	
	if(initialRow!=0&&gameBoard[initialRow-1][initialColumn]!='1'&&(gameBoard[initialRow-1][initialColumn]!='K')&&(gameBoard[initialRow-1][initialColumn]!='2')){
		
	
		
		if((gameBoard[initialRow-1][initialColumn]=='C')){
			*flag=1;
			gameBoard[initialRow-1][initialColumn]='X';
		}
		if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]='2';
			}
	  
		
		if(gameBoard[initialRow-1][initialColumn]!=0&&gameBoard[initialRow-1][initialColumn]!='G'){
			
				for(i=0;i<ITERASYON;i++){
				buffer[i][initialRow-1][initialColumn]=0;
			}
	  	stack[*numberOfElements]=gameBoard[initialRow-1][initialColumn];
	  	*numberOfElements=*numberOfElements+1;
	  	
	  }
	  temp=0;
	  gameBoard[initialRow-1][initialColumn]='X';
	  if(gameBoard[initialRow-1][initialColumn]=='G'){
	  	temp='G';
	  }
	   *iterasyon=*iterasyon+1;
	recursiveAuto(gameBoard ,initialRow-1, initialColumn,row,column,temp,stack,numberOfElements,flag,buffer,iterasyon);
	  //recursvie cagri
			//yukari
		}
		
		
	if(initialColumn!=0&&gameBoard[initialRow][initialColumn-1]!='1'&&(gameBoard[initialRow][initialColumn-1]!='K')&&(gameBoard[initialRow][initialColumn-1]!='2')){
			
	
		if((gameBoard[initialRow][initialColumn-1]=='C')){
			*flag=1;
			gameBoard[initialRow][initialColumn-1]='X';
			
		}
		if(temp=='G'){
	    		gameBoard[initialRow][initialColumn]=temp;
	    		temp = 0;
			}else if(temp == 0){
				gameBoard[initialRow][initialColumn]='2';
			}
		
		if(gameBoard[initialRow][initialColumn-1]!=0&&gameBoard[initialRow][initialColumn-1]!='G'){
			
				for(i=0;i<ITERASYON;i++){
					
				buffer[i][initialRow][initialColumn-1]=0;
			}
	  	stack[*numberOfElements]=gameBoard[initialRow][initialColumn-1];
	  	*numberOfElements=*numberOfElements+1;
	  }
	  
	  gameBoard[initialRow][initialColumn-1]='X';
	  if(gameBoard[initialRow][initialColumn-1]=='G'){
	  	temp='G';
	  }
	   *iterasyon=*iterasyon+1;
	   recursiveAuto(gameBoard ,initialRow, initialColumn-1,row,column,temp,stack,numberOfElements,flag,buffer,iterasyon);
	  //recursvie cagri
		//sol
	}
	
	}
	
	if(*flag==0){//eger hicbir yone gidilmediyse ve cýkýsa ulasýlmadýysa bi önceki duruma geri dönerek baska yollar aramak adýna buffer dizisini kullandým.
		*iterasyon = *iterasyon-1;
		
		for(i=0;i<row;i++){
	      for(j=0;j<column;j++){
		gameBoard[i][j]=buffer[*iterasyon][i][j];
	         }
        }
    drawBoard(gameBoard,row,column);
	printf("\n\n\n");
	printf("\n\n\n");

	}
	
	return;
}

