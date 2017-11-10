#include <ao/ao.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <caca.h>
#include <signal.h>
#include <sys/wait.h>
#define _GNU_SOURCE
//*constantes e variáveis globais para sintetizar a onda, modifique a vontade para ver o que acontece(não coloque valores muito altos >(5) nas amplitudes de cada onda):

double s1=0.2, s2=0.3, s3=0.5; //Constantes que aplicam sobre os senos
double c1=0, c2=0, c3=0; // Constantes ' '   '   '   '   '  cossenos
double sa=0, sd=0; //modulam senóides em defasagem
double ca=0, cd=0; //ca=amplitude, cd=defazagem de 1 (0) até 1 (2pi)
double sq=1; // amplitude da onda quadrada
double m1=-0.001; //modificador de frequência diminuindo-a se negativo e dando acréscimo se positivo
double t1=0, t2=0; //onda triangular, t1 está a 90 graus de defazagem com t2 (que são suas amplitudes)
double r1=0, r2=0; //ante para ondas rampa defasadas 90 similarmente
double p1=0, pw=100, p2=0, pe=0 ; // onda pulso amplitude(p1 2 p2) e pw & pe= pulse width( range 0 - 200 and 10-> 190, 20-> 180 and so on)
double fi=2; //fade in para entrada da nota, o quão suavemente a nota chega
double fo=10; //fade out para nota.
double tr=0, tf=0; //amplitude de tremulação e sua frequencia(não está em hertz)
double ti=0.1, to=0.3; //Tempo percentual de entrada e saída
double A=10; //amplitude
int oldstyle=1; //sound with fork
double tl=0.4;
int ph=0;

//*************************************************


									//		-----
									//		|	|
double pulse(double wt,double pw){ // ------     -------
		
	    int n=0;
		if (pw!=0){
        while(wt-n*2*M_PI>0){n++;}//calculando o resto de wt/2pi

        double r=(wt-(n-1)*(2*M_PI))/(2*M_PI);//resto
    
        if (r <= pw*5.0/1000.0){
            return 1;
            
        }else{
            return 0;
        }
		}else{
			return 0;
		}
	
	
}
                           //                             ---
double square(double wt){ ///Gera onda de pulso quadrado |   |    |
                                           //                 ----
        int n=0;
        while(wt-n*2*M_PI>0){n++;}//calculando o resto de wt/2pi

        double r=(wt-(n-1)*(2*M_PI))/(2*M_PI);//resto
    
        if (r<=0.5){
            return 1;
            
        }else{
            return -1;
        }
    
    
    
}
                                                    //               /\     */
double triang(double wt){ //função para gerar ondas triangulares    /  \    */
                                      //                 ----
        int n=0;
        while(wt-n*2*M_PI>0){n++;}//calculando o resto de wt/2pi

        double r=(wt-(n-1)*(2*M_PI))/(2*M_PI);//resto
    
        if(r>=0 && r<=0.5){ return (4*r-1)*0.5;}
        else{ return (-4*r+3)*0.5;}    
    
}

double ramp(double wt){ //função para gerar ondas na forma rampa(dente de serra) (/| /)
                                               //                / |/   ----
       int n=0;
        while(wt-n*2*M_PI>0){n++;}//calculando o resto de wt/2pi

        double r=(wt-(n-1)*(2*M_PI))/(2*M_PI);//resto
    
        return (4*r-1)*0.25;
         
           
    
}

static ao_device *sound_dev=NULL;

/*
  Uso: sound(frequencia, duracao);  // frequencia em Hz, duracao em segundos

  Equivalente ao seguinte código feito com funções declaradas em conio.h e dos.h:

    sound(frequencia);
    delay(duracao*1000);  // duracao em milissegundos
    nosound();
*/


int writeconf(){
	char filename[128];
	printf("Enter a name/path for the file to save:  ");
	scanf(" %s", filename);
	strcat(filename, ".ts");
	FILE *cf;
	puts(filename);
	cf=fopen(filename, "w");
	if(cf==NULL){
		printf("Error while oppening!\n");	
		return 1;
	}
	fprintf(cf,"%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%d\n%lf\n%d",s1, s2, s3, c1, c2, c3, sa, sd,  ca, cd,  sq,  m1, t1, t2,r1, r2,p1, pw, p2, pe,fi, fo, tr, tf, ti, to, A, oldstyle, tl, ph);
	fclose(cf);
	return 0;
	
}


int readconf(){
	char filename[128];
	printf("Enter a name/path for the file to import:  ");
	scanf(" %s", filename);
	FILE *cf;
	puts(filename);
	cf=fopen(filename, "r");
	if(cf==NULL){
		printf("Error while oppening file( does it exist?)!\n");
		system("sleep2");
		return 1;
	}
	size_t len = 0;
    ssize_t read;
	double vars[30];
	char * line = NULL;
	int i=0;

    while ((read = getline(&line, &len, cf)) != -1) {
		
		if(i!=27 && i!=29){
		vars[i]=strtod(line,NULL);
		}else
			vars[i]==atoi(line);
        i++;
    }
	s1=vars[0];
	s2=vars[1];
	s3=vars[2];
	c1=vars[3];
	c2=vars[4];
	c3=vars[5];
	sa=vars[6];
	sd=vars[7];
	ca=vars[8];
	cd=vars[9];
	sq=vars[10];
	m1=vars[11];
	t1=vars[12];
	t2=vars[13];
	r1=vars[14];
	r2=vars[15];
	p1=vars[16];
	pw=vars[17];
	p2=vars[18];
	pe=vars[19];
	fi=vars[20];
	fo=vars[21];
	tr=vars[22];
	tf=vars[23];
	ti=vars[24];
	to=vars[25];
	A=vars[26];
	oldstyle=vars[27];
	tl=vars[28];
	ph=vars[29];

    

	
	fclose(cf);
	return 0;
	
}


void sound(double freq, double duration){
	
  pid_t pid;
  pid=fork();
  signal(SIGCHLD, SIG_IGN);
  
  
  if ( pid==0 ) {
  static ao_sample_format format;
  static double sample_period;

  double w=2*M_PI*freq;

  if(!sound_dev){
    int default_driver;

    ao_initialize();
    default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 2;
    format.rate = 44100;
    format.byte_format = AO_FMT_BIG;

    sound_dev = ao_open_live(default_driver, &format, NULL);
    if(!sound_dev){
      fprintf(stderr, "Error opening sound device.\n");
      exit(1);
    }

    sample_period=1./(double)format.rate;
  }

  size_t n_frames=(size_t)(duration*format.rate);
  uint16_t *wave=malloc(format.channels*n_frames*sizeof *wave);
  double a=A;
  double traux=tr*tf/10000;
  
  
  for(size_t i=0; i<n_frames; i++){
    double wt=w*i*sample_period;
    
    
    //Função de modulação: ******************************************
    
    if(fi!=0 || fo!=0 ){ //modula a amplitude 
        double fact=n_frames;
        
        if(ti*fact>i)
            a=(1-pow(2,-fi*i/1000));
        
        else if(to*fact>fact-i)
            a=pow(2,-fo*i/1000)*A; 
        
        else
            a=A;
     
    }
    
    if(tr!=0){ //tremulação
        wt+=traux;        
    }
    
    int16_t sample=(int16_t)(a*(10*(s1*sin(wt)+c1*cos(wt)+c2*cos(2*wt)+s2*sin(3*wt)+c3*cos(4*wt)+s3*sin(5*wt) + sa*sin(wt+sd*2*M_PI)+ca*cos(wt+cd*2*M_PI)) + 10*(t1*triang(wt) + t2*triang(wt+M_PI) + r1*ramp(wt)+r2*ramp(wt+M_PI)) + sq*square(wt)+p1*pulse(wt,pw)+p2*pulse(wt,pe)));
   
	if (m1!=0){   w+=m1;    }
    
     if(tr!=0){ //tremulação
         if(traux<tr && traux > -tr){
            traux+=tf; 
         }else if(traux>tr || traux < -tr){
            traux-=tf;
         }
    }
    

    
    
    //fim da modulação; ********************************************8
    
    
    
    
    for(int channel=0; channel<format.channels; channel++)
      wave[i*format.channels+channel]=sample;
  }

  ao_play(sound_dev, (char *)wave, format.channels*n_frames*sizeof *wave);

  free(wave);
  exit(0);
}
}

int changeglobal(){  //funçao chamada dentro de janela ncurse	
	clear();
	char opt[2];
	system("clear");
	while(1){
		system("clear");
		double value;
	printf("Enter the name of the constant you want to change\n\n");	
	printf("tl for time lenght for a note\n");
	printf("---------------------------------------------------------------\n");
	printf("s1 s2 s3 for sin coeficients\n");	
	printf("---------------------------------------------------------------\n");
	printf("c1 c2 c3 for cos coeficients\n");
	printf("---------------------------------------------------------------\n");
	printf("sa sin wave sd to set its phase shift (from 0 to 1) \n");	
	printf("---------------------------------------------------------------\n");
	printf("ca cd the same with cos\n");
	printf("---------------------------------------------------------------\n");
	printf("sq to set the square wave amplitude\n");
	printf("---------------------------------------------------------------\n");
	printf("m1 to set the frequency changer\n");
	printf("---------------------------------------------------------------\n");
	printf("t1 and t2 for triangular waves amplitude\n");
	printf("---------------------------------------------------------------\n");
	printf("r1 and r2 for sawtooth wave amplitudes\n");
	printf("---------------------------------------------------------------\n");
	printf("p1,p2 and pw,pe for pulse wave amplitude and its width (from 0 to 100)\n");
	printf("---------------------------------------------------------------\n");
	printf("fi for coeficient of exponencial amplitude elevation and ti for its percentual duration\n");
	printf("---------------------------------------------------------------\n");
	printf("fo for coeficient of exponencial amplitude decay and to for its percentual duration\n");
	printf("---------------------------------------------------------------\n");
	printf("tr amplitude noise and tf for noise frequency\n");
	printf("---------------------------------------------------------------\n");
	printf("A for overall amplitude\n");
	printf("---------------------------------------------------------------\n");
	printf("0 to set all variables to 0\n");
	printf("---------------------------------------------------------------\n");
	printf("1 to print all variables values\n");
	printf("---------------------------------------------------------------\n");
	printf("L or l to left this and get back to the keyboard\n");
	printf("---------------------------------------------------------------\n");
	printf("ph for ajust the pitch( enter an integer number to move the key positioning of notes)\n");
	printf("---------------------------------------------------------------\n\n");
	
	
	scanf(" %s",opt);
	
	if (opt[0]=='l' || opt[0]=='L' )
		return 1;
	if(opt[0]!='0' && opt[0]!='1')
	{	printf("Enter the value: ");
		scanf(" %lf", &value);
	}
	switch (opt[0]){
		case 's':
			switch (opt[1]){
				case '1':
					s1=value;
					break;
					
				case '2':
					s2=value;
					break;
				
				case '3':
					s3=value;
					break;
					
				case 'a':
					sa=value;
					break;
					
				case 'd':
					sd=value;
					break;
				case 'q':
					sq=value;
					break;
			}
			break;
			
		case 'c':
			switch (opt[1]){
				case '1':
					c1=value;
					break;
					
				case '2':
					c2=value;
					break;
				
				case '3':
					c3=value;
					break;
					
				case 'a':
					ca=value;
					break;
					
				case 'd':
					cd=value;
					break;
			}
			break;
			
				case 'm':
			switch (opt[1]){
				case '1':
					m1=value;
					break;

			}
			break;
		case 't':
			switch (opt[1]){
				case '1':
					t1=value;
					break;
					
				case '2':
					t2=value;
					break;
				
				case 'r':
					tr=value;
					break;
					
				case 'f':
					tf=value;
					break;
					
				case 'l':
					tl=value;
					break;
                                        
                                case 'i':
                                        ti=value;
                                        break;
				
                                        
                                case 'o':
                                        to=value;
                                        break;
			}
			break;
	
		case 'r':
			switch (opt[1]){
				case '1':
					r1=value;
					break;
					
				case '2':
					r2=value;
					break;
				
			}
			break;
		case 'p':
			switch (opt[1]){
				case '1':
					p1=value;
					break;
					
				case 'w':
					pw=value;
					
					break;
				
				case 'e':
					pe=value;
					break;
					
				case '2':
					p2=value;
					break;
					
					
				case 'h':
					ph=value;
					break;				

			}
			break;
		
			case 'f':
			switch (opt[1]){
				case 'i':
					fi=value;
					break;
					
				case 'o':
					fo=value;
					break;

			}
			break;
			
			case 'A':
				A=value;
	
			break;
			
			case '0':
				
				s1=0; s2=0; s3=0; //Constantes que aplicam sobre os senos
				c1=0; c2=0; c3=0; // Constantes ' '   '   '   '   '  cossenos
				sa=0; sd=1; //modulam senóides em defasagem
				ca=0; cd=0; //ca=amplitude; cd=defazagem de 0 (0) até 1 (2pi)
				sq=0; // amplitude da onda quadradaprintf("sa=%f sd=f%", sa, sd);
				m1=0; //modificador de frequência diminuindo-a se negativo e dando acréscimo se positivo
				t1=0; t2=0; //onda triangular; t1 está a 90 graus de defazagem com t2 (que são suas amplitudes)
				r1=0; r2=0; //ante para ondas rampa defasadas 90 similarmente
				p1=0; pw=0; // onda pulso amplitude e pw = pulse width( range 0 - 200 and 10-> 190; 20-> 180 and so on)
				fi=0; ti=0; //fade in para entrada da nota; o quão suavemente a nota chega
				fo=0; to=0; //fade out para nota.
				tr=0; tf=0; //amplitude de tremulação e sua frequencia(não está em hertz)
				A=10; //amplitude
				ph=0;
				break;
				
			case '1':
				
				printf("s1=%f s2=%f s3=%f\n", s1, s2, s3);
				printf("c1=%f c2=%f c3=%f""\n", c1, c2, c3);
				printf("sa=%f sd=%f\n", sa, sd);
				printf("ca=%f cd=%f\n", ca, cd);
				printf("sq=%f\n", sq);
				printf("m1=%f\n", m1);
				printf("t1=%f t2=%f\n", t1, t2);
				printf("r1=%f r2=%f\n", r1, r2);
				printf("p1=%f pw=%f\n", p1, pw);
				printf("fi=%f fo=%f\n", fi, fo);
                                printf("ti=%f to=%f\n", ti, to);
				printf("tr=%f tf=%f\n", tr, tf);
				printf("A=%f\n", A);
                                printf("tl=%f\n", tl);
								printf("ph=%d\n", ph);
				
				system("sleep 4");
				break;
				
		
	}
	/***
  s1=0, s2=0, s3=0; //Constantes que aplicam sobre os senos
  c1=0, c2=0, c3=0; // Constantes ' '   '   '   '   '  cossenos
  sa=0, sd=1; //modulam senóides em defasagem
  ca=0, cd=0; //ca=amplitude, cd=defazagem de 0 (0) até 1 (2pi)
  sq=1; // amplitude da onda quadrada
 m1=-0.001; //modificador de frequência diminuindo-a se negativo e dando acréscimo se positivo
  t1=0, t2=0; //onda triangular, t1 está a 90 graus de defazagem com t2 (que são suas amplitudes)
  r1=0, r2=0; //ante para ondas rampa defasadas 90 similarmente
  p=2, pw=10; // onda pulso amplitude e pw = pulse width( range 0 - 200 and 10-> 190, 20-> 180 and so on)
 fi=10; //fade in para entrada da nota, o quão suavemente a nota chega
 fo=0.8; //fade out para nota.
 tr=0, tf=0; //amplitude de tremulação e sua frequencia(não está em hertz)
 A=10; //amplitude
	
	}
	*****/
}
}

double getnf(int pitch, char n){
    
    /* Uso: getnf("c" ) retorna a frequência da primeira nota Dó. Para utilizar oitavas superiores pasta multiplicar a saída da função pelo número, por exemplo: queremos a nota D2, basta fazer 2*getnf('d'). 
     
     Para notas sustenidas, basta escrever o maiúsculo da nota correspondente, por exemplo, getnf("C") representa dó sustenido
     */
    
    int b;
    switch(n){
        
        case 'c':
            b=0;
            break;
        case 'C':
            b=1;
            break;
        case 'd':
            b=2;
            break;
        case 'D':
            b=3;
            break;
        case 'e':
            b=4;
            break;
        case 'E':
            b=5;
            break;
        case 'f':
            b=5;
            break;
        case 'F':
            b=6;
            break;
        case 'g':
            b=7;
            break;
        case 'G':
            b=8;
            break;
        case 'a':
            b=9;
            break;
        case 'A':
            b=10;
            break;
        case 'b':
            b=11;
            break;
        case 'B':
            b=12; 
            break;
        
    }
    double freq=pow(2.0, (b+pitch)/12.0)*32.703;      

    return freq;
}


int doesFileExist(const char* filename)
{
  FILE* fptr = fopen(filename, "r");
  if (fptr != NULL)
  {
    fclose(fptr);
    printf("File exists\n");
    return 1;
  }
  printf("File doesnt exist\n");
  return 0;
}


void record(){
clear();
endwin();
system("clear");

	printf("Not avaiable yet!\n");
	sleep(1);


initscr();
keypad(stdscr, TRUE);
noecho();
curs_set(FALSE);	
}


void play(){
clear();
endwin();

system("clear");

	
while(1){
	char path[128];
	char type='a';
	char run[200];
	printf("Do you want to play with cvlc(only if you have vlc installed)(v) or with aplay?(a):  ");
	scanf(" %c", &type);
	printf("\n");
	printf("Enter the file/path to be played:  ");
	scanf(" %s", path);
	if(!doesFileExist(path)){
		printf("\n\n\n");
		continue;
	}
	if(type=='v'){
		strcpy(run, "cvlc ");
		strcat(run, path);
		strcat(run, " &");
		system(run);
		break;
	} else if(type=='a'){
		strcpy(run, "aplay ");
		strcat(run, path);
		strcat(run, " &");
		system(run);
		break;
	} 
	else{
		printf("Please! Enter a valid option!\n\n\n");
	}
}

initscr();
keypad(stdscr, TRUE);
noecho();
curs_set(FALSE);			
	
}


caca_canvas_t *cv;
caca_display_t *dp;

int keyboard(){
	
	int input;
	cv = caca_create_canvas(80, 24);
    if(cv == NULL)
    {
        printf("Failed to create canvas\n");
        return 1;
    }

    dp = caca_create_display(cv);
    if(dp == NULL)
    {
        printf("Failed to create display\n");
        return 1;
    }

	char played;
	char sharp=' ';
	played='\0';
	int time=0;
	
	do
 {
	printw("#################################################################################################################\n                             ----Terminal-----Synth----- \n\n\n\n\n\n\n");
	printw("                                      %c", played);
	printw("%c \n\n\n\n\nHit [ to save your configuration, ] to import a configuration, L to enter the configuration menu, - to start recording,\n = to play a record, esc to exit\n #################################################################################################################", sharp);
	usleep(1);
    nodelay(stdscr, TRUE);
    input = getch();

	switch(input)
    {
		
		case '-':
			
			record();
			
			
			break;
			
		case '=':
			
			play();
			
			break;
			
		case 27:
			
			endwin();
           	return 0;
			
			
        case 'a':
		sound(getnf(ph,'b')*2,tl);
		played='B';
		sharp=' ';
		time=0;
		break;

		case 'z':
		sound(getnf(ph,'c')*4,tl);
		played='C';
		sharp=' ';
		time=0;
        break;

		case 's':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*4,tl);
        break;
		
		case 'x':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*4,tl);
        break;
		
		case 'd':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*4,tl);
        break;
		
		case 'c':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*4,tl);
        break;
		
		case 'f':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'E')*4,tl);
        break;
		
		case 'v':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'f')*4,tl);
        break;
		
		case 'g':
		played='F';
		sharp='#';
		time=0;
		sound(getnf(ph,'F')*4,tl);
        break;
		
		case 'b':
		played='G';
		sharp=' ';
		time=0;
		sound(getnf(ph,'g')*4,tl);
        break;
		
		case 'h':
		played='G';
		sharp='#';
		time=0;
		sound(getnf(ph,'G')*4,tl);
        break;
		
		case 'n':
		played='A';
		sharp=' ';
		time=0;
		sound(getnf(ph,'a')*4,tl);
        break;
		
		case 'j':
		played='A';
		sharp='#';
		time=0;
		sound(getnf(ph,'A')*4,tl);
        break;
		
		case 'm':
		played='B';
		sharp=' ';
		time=0;
		sound(getnf(ph,'b')*4,tl);
        break;
		
		case 'k':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*8,tl);
        break;
		
		case '1':
		sound(getnf(ph,'b')*4,tl);
		played='B';
		sharp=' ';
		time=0;
		break;

		case 'q':
		sound(getnf(ph,'c')*8,tl);
		played='C';
		sharp=' ';
		time=0;
        break;

		case '2':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*8,tl);
        break;
		
		case 'w':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*8,tl);
        break;
		
		case '3':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*8,tl);
        break;
		
		case 'e':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*8,tl);
        break;
		
		case '4':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'E')*8,tl);
        break;
		
		case 'r':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'f')*8,tl);
        break;
		
		case '5':
		played='F';
		sharp='#';
		time=0;
		sound(getnf(ph,'F')*8,tl);
        break;
		
		case 't':
		played='G';
		sharp=' ';
		time=0;
		sound(getnf(ph,'g')*8,tl);
        break;
		
		case '6':
		played='G';
		sharp='#';
		time=0;
		sound(getnf(ph,'G')*8,tl);
        break;
		
		case 'y':
		played='A';
		sharp=' ';
		time=0;
		sound(getnf(ph,'a')*8,tl);
        break;
		
		case '7':
		played='A';
		sharp='#';
		time=0;
		sound(getnf(ph,'A')*8,tl);
        break;
		
		case 'u':
		played='B';
		sharp=' ';
		time=0;
		sound(getnf(ph,'b')*8,tl);
        break;
		
		case '8':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*16,tl);
        break;
		
				
		case 'i':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*16,tl);
        break;
		
				
		case '9':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*16,tl);
        break;
				
		case 'o':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*16,tl);
        break;
				
		case '0':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*16,tl);
        break;
				
		case 'p':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*16,tl);
        break;
		
		
		//capsbass
		
		 case 'A':
		sound(getnf(ph,'b')*1,tl);
		played='B';
		sharp=' ';
		time=0;
		break;

		case 'Z':
		sound(getnf(ph,'c')*1,tl);
		played='C';
		sharp=' ';
		time=0;
        break;

		case 'S':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*1,tl);
        break;
		
		case 'X':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*1,tl);
        break;
		
		case 'D':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*1,tl);
        break;
		
		case 'C':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*1,tl);
        break;
		
		case 'F':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'E')*1,tl);
        break;
		
		case 'V':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'f')*1,tl);
        break;
		
		case 'G':
		played='F';
		sharp='#';
		time=0;
		sound(getnf(ph,'F')*1,tl);
        break;
		
		case 'B':
		played='G';
		sharp=' ';
		time=0;
		sound(getnf(ph,'g')*1,tl);
        break;
		
		case 'H':
		played='G';
		sharp='#';
		time=0;
		sound(getnf(ph,'G')*1,tl);
        break;
		
		case 'N':
		played='A';
		sharp=' ';
		time=0;
		sound(getnf(ph,'a')*1,tl);
        break;
		
		case 'J':
		played='A';
		sharp='#';
		time=0;
		sound(getnf(ph,'A')*1,tl);
        break;
		
		case 'M':
		played='B';
		sharp=' ';
		time=0;
		sound(getnf(ph,'b')*1,tl);
        break;
		
		case 'K':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*2,tl);
        break;
		
		case '!':
		sound(getnf(ph,'b')*2,tl);
		played='B';
		sharp=' ';
		time=0;
		break;

		case 'Q':
		sound(getnf(ph,'c')*2,tl);
		played='C';
		sharp=' ';
		time=0;
        break;

		case '@':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*2,tl);
        break;
		
		case 'W':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*2,tl);
        break;
		
		case '#':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*2,tl);
        break;
		
		case 'E':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*2,tl);
        break;
		
		case '$':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'E')*2,tl);
        break;
		
		case 'R':
		played='F';
		sharp=' ';
		time=0;
		sound(getnf(ph,'f')*2,tl);
        break;
		
		case '%':
		played='F';
		sharp='#';
		time=0;
		sound(getnf(ph,'F')*2,tl);
        break;
		
		case 'T':
		played='G';
		sharp=' ';
		time=0;
		sound(getnf(ph,'g')*2,tl);
        break;
		
		case '¨':
		played='G';
		sharp='#';
		time=0;
		sound(getnf(ph,'G')*2,tl);
        break;
		
		case 'Y':
		played='A';
		sharp=' ';
		time=0;
		sound(getnf(ph,'a')*2,tl);
        break;
		
		case '&':
		played='A';
		sharp='#';
		time=0;
		sound(getnf(ph,'A')*2,tl);
        break;
		
		case 'U':
		played='B';
		sharp=' ';
		time=0;
		sound(getnf(ph,'b')*2,tl);
        break;
		
		case '*':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*4,tl);
        break;
		
				
		case 'I':
		played='C';
		sharp=' ';
		time=0;
		sound(getnf(ph,'c')*4,tl);
        break;
		
				
		case '(':
		played='C';
		sharp='#';
		time=0;
		sound(getnf(ph,'C')*4,tl);
        break;
				
		case 'O':
		played='D';
		sharp=' ';
		time=0;
		sound(getnf(ph,'d')*4,tl);
        break;
				
		case ')':
		played='D';
		sharp='#';
		time=0;
		sound(getnf(ph,'D')*4,tl);
        break;
				
		case 'P':
		played='E';
		sharp=' ';
		time=0;
		sound(getnf(ph,'e')*4,tl);
        break;
		
		//capsbass
		
		case 'l':
			endwin();
			changeglobal();
			initscr();
				initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);
	 sharp=' ';
	played='\0';
	time=0;
			
			break;
		
		case 'L':
			endwin();
			changeglobal();
			initscr();
				initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);
	
	sharp=' ';
	played='\0';
	time=0;
			
			
			break;
			
		case 91:
			endwin();
			system("clear");
			
			writeconf();
			initscr();
			keypad(stdscr, TRUE);
			noecho();
			curs_set(FALSE);
			
		case 93:
			endwin();
			system("clear");
			
			readconf();
			initscr();
			keypad(stdscr, TRUE);
			noecho();
			curs_set(FALSE);

    }
 
	
	if (played!='\0'){
		time++;
		if(time>3000)
			played=' ';
			//sharp=' ';
			time=0;
	}
	
	input=0;
	clear();
	
}while(1);
    endwin();
}

// void record()
int main(){		
	//sound(getnf(ph,'c')*6, 1);
	
	//playredezvouz4();
	
keyboard();
system("clear");
return 0;

}
