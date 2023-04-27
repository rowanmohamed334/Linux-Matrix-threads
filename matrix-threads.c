#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//global variable to know the size of the input
int i = 0;

//indeces of the matrices
int R;
int C;
int R1;
int C1;
int buff[2800];
int sum_e = 0;
int sum_r = 0;

//intilization of the matrices
int A[600][600];
int B[600][600];
int result_r[600][600];
int result_e[600][600];

//structure for matrices elements
struct Elements{
	int row;
	int col;
};

void *multiply_numbers(void *ptr);
void *multiply_rows(void *ptr);

//function to read input from file
int *read_input(){
	FILE *fp;
	int N;
	int M;
	fp = fopen("input3-matrix.txt", "r");
	while(fscanf(fp, "%d", &buff[i])!=EOF){
		i++;
		R = buff[0];
		C = buff[1];
		M = (R * C)+2;
		N = (R * C)+3;
		R1 = buff[M];
		C1 = buff[N];
	}
	fclose(fp);	
	return buff;
}


void matrices_formation(int *array){
	//initilization of the two matrices
	A[R][C];
	B[R1][C1];
	result_r[R][C1];
	result_e[R][C1];
	//number of threads
	int nth = R*C1;
	int rth = R;
	pthread_t ths[nth];
	pthread_t thr[rth];
	//variables to calculate time
	clock_t t,t1;
   	
 
	//the output file
		FILE *wfpr = fopen("output_matrix.txt", "w");
		if(wfpr == NULL){
			printf("could not open the file\n");
		}
	if(C == R1){
		
		//first matrix
		i = 2;
		for(int j=0; j<R; j++){
			for(int k=0; k<C; k++){
			A[j][k] = array[i];
			i++;
			}
		}
		
		//second matrix
		i = i+2;
		for(int z=0; z<R1; z++){
			for(int t=0; t<C1; t++){
			B[z][t] = array[i];
			i++;
			}
		}
		
	////////////////////////////////*Matrix Multiplication with threads per elements*////////////////////////////////////////////
		fprintf(wfpr, "BY ELEMENT\n");
		t = clock();
		int ind = 0;
		for(int d=0; d<R; d++){
			for(int q=0; q<C1; q++){
				//sum_e = 0;
				struct Elements *elements = (struct Elements*) malloc(sizeof(struct Elements));
				elements->row = d;
				elements->col = q;
				//pthread_t ths;
				pthread_create(&ths[ind], NULL, multiply_numbers, elements);
				pthread_join(ths[ind], NULL);
				ind++;
			}
		}
		t = clock() - t;
   		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		
		//writing the result in the output file
		for(int d=0; d<R; d++){
			for(int q=0; q<C1; q++){
			fprintf(wfpr, "%d ", result_e[d][q]);
			}
			fprintf(wfpr, "\n");
		}
		fprintf(wfpr, "Time:%f\n", time_taken);
		fprintf(wfpr, "\n");
		
	////////////////////////////////*Matrix Multiplication with threads per rows*////////////////////////////////////////////
		fprintf(wfpr, "BY ROW\n");
		t1 = clock();
		int ind2 = 0;
		for(int d=0; d<R; d++){
			struct Elements *rows = (struct Elements*) malloc(sizeof(struct Elements));
			rows->row = d;
			//rows->col = q;
			//pthread_t thr;
			pthread_create(&thr[ind2], NULL, multiply_rows, rows);
			pthread_join(thr[ind2], NULL);
			ind2++;
		}
		t1 = clock() - t1;
   		double time_taken1 = ((double)t1)/CLOCKS_PER_SEC;
   		
		for(int d=0; d<R; d++){
			for(int q=0; q<C1; q++){
			fprintf(wfpr, "%d ", result_r[d][q]);
			}
			fprintf(wfpr, "\n");
		}
		fprintf(wfpr, "Time:%f\n", time_taken1);
		fprintf(wfpr, "\n");
	}
	else{
		fprintf(wfpr, "MULTIPLICATION OF MATRIX IS NOT POSSIBLE");
	}
}

//thread for each element
void *multiply_numbers(void *ptr){
	sum_e = 0;
	struct Elements *l = ptr;
	for(int r=0; r<R1; r++){
		int n1 = A[l->row][r];
		int n2 = B[r][l->col];
		sum_e += A[l->row][r]*B[r][l->col];
		
	}
	result_e[l->row][l->col] = sum_e;
	pthread_exit(0);
		
}

//thread for each row
void *multiply_rows(void *ptr){
	for(int q=0; q<C1; q++){
		sum_r = 0;
		struct Elements *l = ptr;
		for(int r=0; r<R1; r++){
			int n1 = A[l->row][r];
			int n2 = B[r][q];
			sum_r += n1*n2;
			result_r[l->row][q] = sum_r;
			
		}
		
	}
	pthread_exit(0);	
}


int main(){
	int *input;
	input = read_input();
	matrices_formation(input);
	return 0;
}
