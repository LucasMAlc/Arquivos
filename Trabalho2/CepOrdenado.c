#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct _Endereco Endereco;

struct _Endereco {
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];

	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void* c1, const void* c2){
    
    return strncmp(((Endereco*)c1)->cep,((Endereco*)c2)->cep,8);
}

int main(int argc, char**argv) {
	long tam, quant1, quant2, partes;
	FILE *f = fopen("cep.dat", "r"), *arq;
	Endereco *e;

	fseek (f, 0, SEEK_END);
    tam = ftell(f);
	quant1 = tam/sizeof(Endereco);
	partes = quant1/8;
	rewind(f);

	printf("--Dividindo e Gerando arquivos--\n");

	for (int i = 1; i <= 8; i++){
        if (i < 8){
			quant2 = partes;
		}
        else{ 
			quant2 = quant1 - (partes * 7);
		}

        e = (Endereco*) malloc(quant2*sizeof(Endereco));

        char arquivo[200];
        sprintf(arquivo,"cep_%d.dat",i);

        arq = fopen (&arquivo, "w");

        fread(e, sizeof(Endereco), quant2, f);
        qsort(e,quant2,sizeof(Endereco),compara);
        fwrite (e, sizeof(Endereco), quant2, arq);

        fclose(arq);
        free(e);
    }

    int x = 1, y = 9;
	FILE *arq1, *arq2, *arq3;
    Endereco e2, e3;

    printf("--Intercalando arquivos--\n");

	while (y < 16){
		char aux1[200], aux2[200], inter[200];

        sprintf(aux1,"cep_%d.dat",x++);
        sprintf(aux2,"cep_%d.dat",x++);
        if (y==15){
			sprintf(inter,"CepOrdenado.dat", y++);
		}
        else {
			sprintf(inter,"cep_%d.dat", y++);
		}

        arq1 = fopen(aux1,"r");
        arq2 = fopen(aux2,"r");
        arq3 = fopen(inter,"w");

        fread(&e2,sizeof(Endereco),1,arq1);
	    fread(&e3,sizeof(Endereco),1,arq2);

        while(!feof(arq1) && !feof(arq2)){
            if(compara(&e2,&e3) < 0){
                fwrite(&e2,sizeof(Endereco),1,arq3);
                fread(&e2,sizeof(Endereco),1,arq1);
            }
            else{
                fwrite(&e3,sizeof(Endereco),1,arq3);
                fread(&e3,sizeof(Endereco),1,arq2);
            }
        }

        while(!feof(arq1)){
            fwrite(&e2,sizeof(Endereco),1,arq3);
            fread(&e2,sizeof(Endereco),1,arq1);		
        }

        while(!feof(arq2)){
            fwrite(&e3,sizeof(Endereco),1,arq3);
            fread(&e3,sizeof(Endereco),1,arq2);		
        }

    	fclose(arq1);
		fclose(arq2);
		fclose(arq3);
    }
    printf("Arquivo ordenado com sucesso.\n");
}
