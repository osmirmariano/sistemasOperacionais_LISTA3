#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <time.h>

#define numAcentos 4 //Quantidade de acentos disponíveis de acordo com o problema proposto
#define tempDormirMax 2 //Definindo o tempo de dormir.


//Criando as variáveis semáforos 
sem_t estudante;  //semáforo estudante
sem_t monitor;  //semáforo monitor

//sem_t mutex;
pthread_mutex_t mutex; //criando mutex para evitar que dois processo ou threads tenha acesso simultâneo a um recurso compartilhado

int cadeira[4]; //Vetor para armazena a quantidade de cadeiras disponíveis
int count = 0; //Para determinar a quantidade de estudantes em espera, inicialmente são 0 por isso que está setado para 0
int proxAcento = 0;
int proxEnsinar = 0;

//Declaração das funções
void dormirAleatorio();
void *jogando(void* stu_id);
void *monitorEnsinado();

//Função principal
int main(int argc, char **argv){
	//Como cada estudante terá uma thread e é o usuário que irá informar a quantidade de usuário, a pthread é declarada do tipo ponteiro
	//E também o monitor terá sua thread
	pthread_t *estudantes;
	pthread_t monitorEnsina;
	
	int *idEstudante, numEstudante, i;

	printf("Informe a quantidade de estudantes: ");
	scanf("%d", &numEstudante);

	//Criando
	estudantes = (pthread_t*)malloc(sizeof(pthread_t) *numEstudante);
	idEstudante = (int*)malloc(sizeof(int) *numEstudante);

	//sintaxe do método: memset (void *fonte , o valor int, size_t num) ; 
	//Ele define um bloco especificado de memória para um caractere especificado, sobrescrevendo os dados originais
	memset(idEstudante, 0, numEstudante); //0 é atribuído a numEstudante
	//Inicializando os semáforos criados globalmente
	sem_init(&estudante,0,0); 
	sem_init(&monitor,0,1);
	
	srand(time(NULL));

	//Inicializando mutex
	pthread_mutex_init(&mutex,NULL);

	//criando pthread
	pthread_create(&monitorEnsina, NULL, monitorEnsinado, NULL);

	//O for está criando as threads para todos os estudantes que o usuário informou
	for(i = 0; i < numEstudante; i++){
		idEstudante[i] = i+1;
		pthread_create(&estudantes[i], NULL, jogando, (void*) &idEstudante[i]);
	} 
	//Esperando o monitor terminar de ensinar
	pthread_join(monitorEnsina, NULL);
	//Estudantes na fila esperando vagas para pedi ajuda
	for(i = 0; i < numEstudante; i++){
		pthread_join(estudantes[i],NULL);
	}	
	return 0;
}


void *jogando(void *stu_id){
	int id = *(int*)stu_id;
	printf("O estudante %d está sendo ajudado.\n",id);		
	//While igual pq se monitor = 1 significa que as threads em espera teoriamente irão dormir
	while(1){
		//Chamando a função para dormir
		dormirAleatorio();

		//Bloqueia mutex
		pthread_mutex_lock(&mutex);
		//Verifica se a quantidade de estudantes é menor que o numero de acentos, visto que acentos só tem 4
		if(count < numAcentos){
			cadeira[proxAcento] = id;
			count++;
			
			printf("O estudante %d está jogando \n", id);
			printf("Os estudantes: [1] %d [2] %d [3] %d [4] %d estão jogando.\n", cadeira[0],cadeira[1],cadeira[2], cadeira[3]);
			proxAcento = (proxAcento+1) % numAcentos;
			
			//Desloqueia o mutex
			pthread_mutex_unlock(&mutex);
			//Desbloqueia o semáforo estudante
			sem_post(&estudante);
			//Bloqueia o monitor
			sem_wait(&monitor);
		}
		//Cao não exista cadeiras disponíveis
		else{
			pthread_mutex_unlock(&mutex);
			printf("Não existe cadeira disponíveis. O estudante %d está jogando \n", id);		
		}
	}				
}

void *monitorEnsinado(){
	while(1){
		//Bloqueia semáforo estudante
		sem_wait(&estudante);	
		pthread_mutex_lock(&mutex);

		printf("O monitor está sanando dúvidas do estudante: %d\n",cadeira[proxEnsinar]);	
		cadeira[proxEnsinar]=0;
		count--;

		printf("Os estudantes: [1] %d [2] %d [3] %d [4] %d estão jogando \n", cadeira[0], cadeira[1], cadeira[2], cadeira[3]);
		proxEnsinar = (proxEnsinar + 1) % numAcentos;

		dormirAleatorio();
		printf("Monitor terminou de sanar dúvidas.\n\n");	
		pthread_mutex_unlock(&mutex);
		//Desbloqueia o semáforo monitor
		sem_post(&monitor);
	}	
}

void dormirAleatorio(){
	int time = rand() % tempDormirMax + 1;
	sleep(time);
}
