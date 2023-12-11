// C++ code
#define imprimir(a) Serial.println(a); //imprimir en el Serial
#define imprimirj(a) Serial.print(a); //imprimir en el Serial junto
int btnDo = 2; //Declaracion de notas musicales (botones)
int btnRe = 3;
int btnMi = 4;
int btnFa = 5;
int btnSol = 6;
int btnLa = 7;
int btnSi = 8;
int btnDoo = 9;
int buzzer = 10;

int btnRec = 11; //Declaracion de botones reproductivos
int btnPlay = 12;
int btnReset = 13;  

int melody[5][5]= {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}; 
//Creacion de array bidimensional para guardar las canciones
String nombre[5]={}; //array para guardar el nombre de las canciones
bool controlAutomatico = 0;
bool controlCiclo = 0; //Controla la funcionalidad del programa
bool controlCiclo2 = 1;
bool controlEdit = 1;
bool controlEdit2 = 1;
int controlNumMelody = 0; //Lleva la cuenta de la cancion actual que
                          //se edita
int controlMelody = 0; //Controla la reproduccion de la cancion actual.
bool controlInterfaz = 0;
bool controlInterfaz2= 0;
String encender;
bool controlSwitch = 1;
bool controlMode = 0;
bool controlSelect = 0;
bool on; //Controla si el dispositivo esta encendido y apagado.
String mode; //Almacena el modo actual del dispositivo.
int select; //Almacena la opcion seleccionada por el usuario
int x = 0; 

void setup() { 
Serial.begin(9600); //baud-rate
  pinMode(buzzer, OUTPUT); //salida
  pinMode(btnDo, INPUT);//Input es para definir que los botones van a ingresar una accion
  pinMode(btnRe, INPUT);//Declaramos los botnes como entradas o salidas
  pinMode(btnMi, INPUT);
  pinMode(btnFa, INPUT);
  pinMode(btnSol, INPUT);
  pinMode(btnLa, INPUT);
  pinMode(btnSi, INPUT);
  pinMode(btnDoo, INPUT);
  pinMode(btnRec, INPUT);
  pinMode(btnPlay, INPUT);
  pinMode(btnReset, INPUT);
}

void turn_on() { //cuando se encienda el arduino imprimira el mensaje
  on = true;
  Serial.println("Se ha prendido");
}

void turn_off() { //cuando se apaga el arduino imprimira el mensaje
  on = false;
  Serial.println("Se ha apagado");
 }

void status_mode() {		//imprime el estado actual del piano a traves del puerto serial
  Serial.println(F("Estado del piano: "));
  imprimir(F("Encendido"));	//La funcion macro F() para almacenar los mensajes en la memoria flash en lugar de en la RAM y ahorrar espacio
  imprimir(F("Modo Manual"));//imprime el estado del piano en el monitor serial
  for(int i=0;i<5;i++){ 	//muestra el numero de cada cancion asi como el nombre
    		imprimirj(F("canciones agregadas: ")); // i es el numero de la cancion que se esta mostrando
    		imprimirj(i+1);
    		imprimirj(".-");
    		imprimirj(" ");
    		imprimir(nombre[i]); //arreglo que contiene los nombres de las canciones agregadas al piano
      		delay(100);
  			}
}

void pianito_mode() { //Funcion que controla el modo de operación del sistema
  while(controlMode==0){//espera a que haya datos disponibles en el puerto serial
    if (Serial.available()) { 
      mode = Serial.readStringUntil('\n'); //Si hay datos disponibles, se lee una cadena de caracteres hasta que se recibe el caracter de nueva línea '\n'
      if(mode=="a"){ //Si "mode" es igual a "a", la variable "controlAutomatico" se establece en 0 y se imprime un mensaje de instrucciones
         controlAutomatico=0;
         imprimir(F("Para salir presione el boton Rec y para apagar Reset"));
            imprimir(F("Toca tu melodia"));
        while(controlAutomatico==0){ //llama a una función llamada "music()", que reproduce una melodía. 
                music();			//controlAutomatico==1 se termino de reproducir la cancion
       }
              
      }
    }else{
     	if(mode=="m"){ //Si "mode" es igual a "m", se imprime un mensaje de instrucciones y se llama a una función llamada "interfaz()".
        imprimir(F("Modo Manual Activado:"));
        interfaz();
        }
     }
     if(digitalRead(btnDo) == HIGH){ //sale del modo "pianito_mode()
   	 controlMode=1; 
     }	
   }
}
  	
void select_mode(){ //verifica si se ha agregado al menos una canción en la variable "controlNumMelody"
  if(controlNumMelody<1){ //Si no hay canciones agregadas, se imprime un mensaje de advertencia.
     imprimir(F("Agregue una cancion antes de seleccionar una canción"));

      }else{
      	
        imprimir(F("Elija el numero de la canción a seleccionar"));
        for(int i=0;i<5;i++){
    		imprimirj(F("canciones agregadas: ")); //se imprime una lista de canciones agregadas, que se almacenan en la matriz "nombre"
    		imprimirj(i+1);
    		imprimirj(".-");
    		imprimirj(" ");
    		imprimir(nombre[i]);
      		delay(100);
  			}
        while(controlSelect==0){ // espera a que haya datos disponibles en el puerto serial a través de la función "Serial.available()
          if(Serial.available()){ 
          	select = Serial.parseInt(); //Si hay datos disponibles, se lee un número entero utilizando la función "Serial.parseInt()
            controlSelect=1; //La variable "controlSelect" se establece en 1 para indicar que se ha seleccionado una canción.
            if(nombre[select-1]==""){ //Si el numero seleccionado no corresponde a una cancion guardada entonces se imprime un mensaje
            	imprimir(F("Selecciona una cancion agregada"));
              	controlSelect=0;
            }
          }
        }
        imprimirj(F("Cancion Seleccionada ")); 
    	imprimir(nombre[select-1]); //Una vez seleccionada una canción, se imprime un mensaje para que el usuario reprodusca, edite o Elimine una cancion
    	imprimir(F("Que desea hacer:"));
    	imprimir(F("si quiere reproducir la cancion presione el boton (Play)"));
    	imprimir(F("si quiere editar la cancion presione el boton (Rec)"));
    	imprimir(F("si quiere eliminar la cancion presione el boton (Reset)"));
    	while(controlSelect==1){  //Se ejecuta otro ciclo "while" que espera a que se presione uno de los botones "Play", "Rec" o "Reset".
          
        	if(digitalRead(btnRec)==HIGH)//llama a una función llamada "editarMelody()", que permite al usuario editar la canción.
  			{
    			Serial.println(F("Reiscribiendo canción")); //
    			controlEdit = 1;
              	controlSelect=0;
    			editarMelody(select-1);
              	
    
  			}
  			if(digitalRead(btnPlay)==HIGH) // llama a una función llamada "reproducirMelody()", que reproduce la canción
  			{
    			Serial.println(F("reproduciendo la melodia"));
              	controlSelect=0;
      			reproducirMelody(select-1);
    			delay(250);
  			}
  			if(digitalRead(btnReset)==HIGH)//llama a una función llamada "delete_mode()", que elimina la canción
  			{	
              Serial.println(F("Borrando melodia..."));   			
    			
              	controlNumMelody--;
              	controlSelect=0;
              	delete_mode(select-1);
    			delay(250);
               
  			}
          delay(250);
          
        }
      }
    }

void add_mode() { //funcion para agregar canciones al dispositivo
  if(on && (controlNumMelody<5)){ //Si esta encendido y si hay menos de 5 cancionese entonces.
     for(int i=0;i<5;i++){	
    	imprimirj(F("canciones agregadas: ")); //muestra las canciones agregadas en el array "nombre"
    	imprimirj(i+1);
    	imprimirj(".-");
    	imprimirj(" ");
    	imprimir(nombre[i]);
      	delay(100);
  		}
 
        Serial.println(F("Agrega el nombre de la cancion")); //crear una nueva cancion
    
        while(controlCiclo==0){
        	if (Serial.available()) {
    	
      			nombre[controlNumMelody] = Serial.readStringUntil('\n'); //lee caracteres del puerto serie y los devuelve en una cadena
          		imprimir(controlNumMelody);
      			imprimir(nombre[controlNumMelody]);
          		controlCiclo=1;
        }
       }
        imprimir(F("Crea la cancion. *Max cinco notas")); //grabar una nueva cancion con maximo 5 notas
    	Serial.println(F("Presiona el boton Rec para terminar"));
    	//Serial.println(controlNumMelody);
        while(controlCiclo2==1){
          
  			if(digitalRead(btnDo) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Do)y la guarda en el arreglo "melody"
    			//Serial.println(controlNumMelody);
    			melody[controlNumMelody][controlMelody]= 261;
    			Serial.print("do ");
    			controlMelody++;
    			//Serial.println(controlMelody);
    			delay(250);
  				}
  
  			if(digitalRead(btnRe) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Re)y la guarda en el arreglo "melody"
              	//Serial.println(controlNumMelody);
    			melody[controlNumMelody][controlMelody]= 293;
   				Serial.print("re ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnMi) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Mi)y la guarda en el arreglo "melody"
              	//Serial.println(controlNumMelody);
    			melody[controlNumMelody][controlMelody]= 329;
    			Serial.print("mi ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnFa) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Fa)y la guarda en el arreglo "melody"
    			melody[controlNumMelody][controlMelody]= 349;
    			Serial.print("fa ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnSol) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Sol)y la guarda en el arreglo "melody"
    			melody[controlNumMelody][controlMelody]= 392;
    			Serial.print("sol ");
   				controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnLa) == HIGH && controlMelody < 5){//lee el valor de una nota musical (La)y la guarda en el arreglo "melody"
    			melody[controlNumMelody][controlMelody]= 440;
    			Serial.print("la ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnSi) == HIGH && controlMelody < 5){//lee el valor de una nota musical (Si)y la guarda en el arreglo "melody"
    			melody[controlNumMelody][controlMelody]= 493;
    			Serial.print("si ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnDoo) == HIGH && controlMelody < 5){ //lee el valor de una nota musical (Do)y la guarda en el arreglo "melody"
    			melody[controlNumMelody][controlMelody]= 523;
    			Serial.print("do+ ");
    			controlMelody++;
    			delay(250);
  				}
    		if(digitalRead(btnRec) == HIGH){ //pulsamos el boton REC para dejar de grabar
              	imprimir(F(" "));
   				Serial.println(F("Terminando de grabar"));
    			controlCiclo2 = 0;
    			controlNumMelody++; // la función aumenta el contador de canciones agregadas 
              	controlMelody = 0; //vuelve a colocar la funcion ControlMelody a 0
              	tone(buzzer, 440, 100); //emite un sonido como representacion de que finalizo el proceso de grabacion
  				}
        }
      controlCiclo=0; //Reinicia las variables para prepararla en la
      controlCiclo2=1; //siguiente vez que llame
      }else{
    		if(on==false){
             Serial.println(F("Apagado")); 
            }
    		if(controlNumMelody>4){
              imprimir("Exedido el numero de canciones ha agregar");//numero de cacnciones excedidas
              }
      };
    
};



void loop(){ //funcion para verofocar si el piano esta encendido
             //o apagado atraves de "on"
  //interfaz(); 
  if(on==false){
  imprimir(F("Desea encender el paino, escriba (si) para encenderlo")); //aqui inicia el programa 
    while(controlInterfaz==0){
      
        	if (Serial.available()) {
      		encender = Serial.readStringUntil('\n');
             imprimir(encender);     	
              if(encender=="si"){
                
              turn_on();
              controlInterfaz = 1; //Se establece en 1 para salir del bucle
              }
        	}
       }
  }else{
  	imprimir(F("#piano encendido# desea modo Automatico(a) o Manual(m)"));
    controlMode = 0;
    pianito_mode();
  }

}

void interfaz(){ //Conjunto de opciones que el usuario puede seleccionar
  
    imprimir(F("Bienvenido al pianon pianito Manual que desea hacer:"));
    imprimir(F("1.-Agregar cancion"));
    imprimir(F("2.-Seleccionar cancion (Reproducir)(Editar)(Eliminar) "));
    imprimir(F("3.-Eliminar todo"));
    imprimir(F("4.-Estado del piano"));
    imprimir(F("5.-Apagar"));
    delay(1000);
  	controlInterfaz2=0;
  
  while(controlInterfaz2==0){
  	if (Serial.available()){
      	x = Serial.parseInt();
    }
    
    if(x==1){
      controlInterfaz2=1; //Se activa en 1 para salir del bucle 
      x=0;
      add_mode();
    }
    if(x==2){
      controlInterfaz2=1;
      x=0;
      select_mode();
    }
    if(x==3){
      controlInterfaz2=1;
      x=0;
      eliminarTodo();
    }
    if(x==4){
      controlInterfaz2=1;
      x=0;
      status_mode();
    }
    if(x==5){
      controlInterfaz = 0;
      controlMode=1;
      controlInterfaz2=1;
      x=0;
      mode="";
      turn_off();
    }
    
  }

}

void reproducirMelody(int numSelect){ //Reproduce la melodia seleccionada del array de melodias
  Serial.println(F("entró a la funcion rep melodia"));
  
  for(int i = 0; i < sizeof(melody[numSelect])/sizeof(int); i++) { //Tamaño del array
    if(melody[numSelect][i] == 0) {
    	break;
    }	//Recibe como argumento numSelect, el cual indica que melodia
    //se va a reproducir
    //El for recorre las notas de la melodia.
    imprimir(numSelect); //imprime el numero de la cancion
    Serial.println(melody[numSelect][i]); //imprime el nombre de la cancion seleccionada
    tone(buzzer, melody[numSelect][i], 400); // Tocar la nota por un cuarto de segundo
    delay(800); // Esperar un momento antes de tocar la siguiente nota
  }
}

void editarMelody(int numSelect){//Funcion que Permite editar una melodia existente 
  //o crear una nueva
  if(controlNumMelody<6){//Si el numero de melodias es menor a 6, entra
    //a un ciclo que espera al usuario seleccione la melodia a editar o borrar.
    	char c = Serial.read();
     	imprimirj(F("Numero de cancion seleccionada: "));
    	imprimir(numSelect+1);
 
        Serial.println(F("Agregar nombre de la cancion"));
    	Serial.println(F("Presiona el boton Rec para terminar"));
    	imprimir(controlCiclo2);
        while(controlEdit==1){
          //imprimir("Entro al ciclo para editar....");
        	if (Serial.available()) {
    	
      			nombre[numSelect] = Serial.readStringUntil('\n');
          		//imprimir(controlNumMelody);
      			imprimir(nombre[numSelect]);
              	controlEdit2=1;
              	controlEdit=0;
        }
       }
    	for(int i=0;i<5;i++){
          melody[numSelect][i]=0;
        }
       imprimir(F("Crea la cancion. *Max cinco notas")); //Opciones para editar la melodia
       while(controlEdit2==1){
  			if(digitalRead(btnDo) == HIGH && controlMelody < 5){
    			//Serial.println(controlMelody);
    			melody[numSelect][controlMelody]= 261; 
    			//Serial.println(controlMelody);
    			Serial.print("do ");
    			controlMelody++;
    			//Serial.println(controlMelody);
    			delay(250);
  				}
  
  			if(digitalRead(btnRe) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 293;
   				Serial.print("re ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnMi) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 329;
    			Serial.print("mi ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnFa) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 349;
    			Serial.print("fa ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnSol) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 392;
    			Serial.print("sol ");
   				controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnLa) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 440;
    			Serial.print("la ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnSi) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 493;
    			Serial.print("si ");
    			controlMelody++;
    			delay(250);
  				}
  
  			if(digitalRead(btnDoo) == HIGH && controlMelody < 5){
    			melody[numSelect][controlMelody]= 523;
    			Serial.print("do+ ");
    			controlMelody++;
    			delay(250);
  				}
    		if(digitalRead(btnRec) == HIGH){
   				Serial.println(F("Terminando de Editar"));
    			controlEdit2 = 0;
              	controlMelody = 0;
              	tone(buzzer, 440, 100);
            }
        }
      controlEdit=0;
      controlEdit2=0;
      }
    
}

void delete_mode(int numSelect){//Elimina una melodiaerrrrrrrrrrrrrrrrrrrrrrrrrrrrr seleccionada por el usuario
  //numSelect indica la melodia que se va a eliminar.
  imprimir(F("Entró a la función elminar"));
  
    for(int i = numSelect; i<5; i++){
      for(int j = 0; j<5; j++){
        melody[i][j] = 0;
        melody[i][j] = melody[i+1][j];
        imprimir(melody[i][j]);
        	if(i==4){
      			melody[i][j]=0;
      		}
      }
      nombre[i] = nombre[i+1];
      if(i==4){
      nombre[i]="";
      }
    }
}

void eliminarTodo(){//Elimina todas las melodias almacenadas en m
  //y el vector n
  imprimir(F("Eliminar Todo"));
  for(int i =0; i<5; i++){
    for(int j=0; j<5; j++){
      melody[i][j] = 0;
    }
    nombre[i]={""};
  }
  	controlNumMelody = 0;
}

void music(){//Funcion principal del programa, el cual ejecuta un bucle infinito
  //verifica si los botones se han presionado y reproduce el tono.
  //Y verifica si los botones de reinicio o de modo automatico
  //se han presionado.
	if(digitalRead(btnDo) == HIGH)
  {
    tone(buzzer, 261, 250);
    Serial.print("do");
    delay(250);
  }
  
  if(digitalRead(btnRe) == HIGH)
  {
    tone(buzzer, 293, 250);
    Serial.print("re");
    delay(250);
  }
  
  if(digitalRead(btnMi) == HIGH)
  {
    tone(buzzer, 329, 250);
    Serial.print("mi");
    delay(250);
  }
  
  if(digitalRead(btnFa) == HIGH)
  {
    tone(buzzer, 349, 250);
    Serial.print("fa");
    delay(250);
  }
  
  if(digitalRead(btnSol) == HIGH)
  {
    tone(buzzer, 392, 250);
    Serial.print("sol");
    delay(250);
  }
  
  if(digitalRead(btnLa) == HIGH)
  {
    tone(buzzer, 440, 250);
    Serial.print("la");
    delay(250);
  }
  
  if(digitalRead(btnSi) == HIGH)
  {
    tone(buzzer, 493, 250);
    Serial.print("si");
    delay(250);
  }
  
  if(digitalRead(btnDoo) == HIGH)
  {
    tone(buzzer, 523, 250);
    Serial.print("doo");
    delay(250);
  }
  if(digitalRead(btnRec) == HIGH){
    controlAutomatico=1;
    delay(250);
    imprimir(F("Saliendo del modo Automatico"));
    imprimir(F("#piano encendido# desea modo Automatico(a) o Manual(m)"));
  }
  if(digitalRead(btnReset) == HIGH){
    controlAutomatico=1;
    controlInterfaz = 0;
    controlMode=1;
    mode="";
    delay(250);
    imprimir(F("Saliendo del modo Automatico"));
    turn_off();
  }

}
