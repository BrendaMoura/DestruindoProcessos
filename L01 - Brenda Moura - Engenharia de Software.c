//Autora: Brenda Cristina Dourado Moura
//Gradução: Engenharia de Software
//Turma: 2
//Matéria: Sistemas Operacionais

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int politicaExclusao = -1;
//-1 - Nenhuma politica de exclusao escolhida
//0 - Processos que tem filhos nao podem ser excluidos
//1 - Quando processos que tem filhos sao excluidos, toda a subarvore a partir do ponto de exclusao deve ser excluida
//2 - Quando processos que tem filhos sao excluidos, os filhos devem ser ligados ao vertice superior - avo
int processoInicial = -1;
//Essa variavel verifica se o usuario já criou o processo inicial

typedef struct Arvore{
    int chave;
    struct Arvore *pai;
    struct Arvore *primFilho;
    struct Arvore *proxIrmao;
}noArvore;

typedef noArvore *no;

//Busca processo
no buscarProcesso(no raiz, int chave){
    if(raiz==NULL)return NULL;
    if(raiz->chave==chave)return raiz;
    no p = raiz->primFilho;
    while(p){
        no resp = buscarProcesso(p, chave);
        if(resp)return resp;
        p = p->proxIrmao;
    }
    return NULL;
}

//Cria um novo processo
no novoProcesso(int chave){
    no novo = (no)malloc(sizeof(noArvore));
    if(novo){
        novo->pai = NULL;
        novo->primFilho = NULL;
        novo->proxIrmao = NULL;
        novo->chave = chave;
        return novo;
    }
}

//Insere um novo processo na arvore
int inserirProcesso(int px,int py, no raiz){
    //Busca o pai do novo processo
    no pai = buscarProcesso(raiz,px);
    if(!pai) return 0;
    no filho = novoProcesso(py);
    //Adiciona o pai dele
    filho->pai = pai;
    no p = pai->primFilho;
    if(!p)pai->primFilho = filho;
    else{
        while(p->proxIrmao){
            p = p->proxIrmao;
        }
        p->proxIrmao = filho;
    }
    return 1;
}

//Cria o processo inicial
no criarProcessoInicial(int chave){
    return (novoProcesso(chave));
}

//Contar quantidade de processos
int quantidadeProcessos(no raiz){
    if(raiz==NULL){
        return 0;
    }
    return (1 + quantidadeProcessos(raiz->primFilho) + quantidadeProcessos(raiz->proxIrmao));
}

//Imprimir processos
void imprimirProcessos(no raiz){
    if(raiz!=NULL){
        printf("%d(", raiz->chave);
        no p = raiz->primFilho;
        while(p){
            imprimirProcessos(p);
            p = p->proxIrmao;
        }
        printf(")");
    }
}

//Destruir processos

//0 - Processos que tem filhos nao podem ser excluidos
int exclusaoTipoZero(int px, no raiz){
    no processo = buscarProcesso(raiz, px);
    if(processo){
        if(processo->primFilho == NULL){
            //Pega o pai do processo
            no pai = processo->pai;
            //Pega os filhos do pai
            no filhosPai = processo->pai->primFilho;

            if(filhosPai->chave==px && filhosPai->proxIrmao==NULL){
                //Unico filho
                //Adicionar o processo filho como NULL
                (*pai).primFilho = NULL;
                free(processo);
                return 1;
            }
            else if(filhosPai->chave==px && filhosPai->proxIrmao!=NULL){
                //Primeiro filho, mas nao o unico
                (*pai).primFilho = filhosPai->proxIrmao;
            }
            else{
                //Nao é o irmao mais novo
                no irmaoNovo = filhosPai;
                while(filhosPai){
                    //Precisa pegar o irmao anterior
                    if(filhosPai->chave==px){
                        (*irmaoNovo).proxIrmao = filhosPai->proxIrmao;
                        break;
                    }
                    irmaoNovo = filhosPai;
                    filhosPai = filhosPai->proxIrmao;
                }
                //Exclui o processo
                free(processo);
            }
            return 1;
        }else{
            printf("\nProcesso nao pode ser excluido");
            return 2;
        }
    }
    return 0;
}

//1 - Quando processos que tem filhos sao excluidos, toda a subarvore a partir do ponto de exclusao deve ser excluida
int exclusaoTipoUm(int px, no raiz){
    no processo = buscarProcesso(raiz, px);
    if(processo){
        //Procurar o pai
        no pai = processo->pai;
        no filhosPai = pai->primFilho;
        if(filhosPai->chave==px && filhosPai->proxIrmao==NULL){
            //Filho unico
            (*pai).primFilho = NULL;
        }
        else if(filhosPai->chave==px && filhosPai->proxIrmao!=NULL){
            //Filho mais novo
            (*pai).primFilho = processo->proxIrmao;
        }
        else{
            //Nao é o filho mais novo
            //Exclui o processo da lista de filhos do pai dele
            no irmaoNovo = filhosPai;
            while(filhosPai){
                //Precisa pegar o irmao anterior
                if(filhosPai->chave==px){
                    (*irmaoNovo).proxIrmao = filhosPai->proxIrmao;
                    break;
                }
                irmaoNovo = filhosPai;
                filhosPai = filhosPai->proxIrmao;
            }
        }

        //Exclui os filhos
        no p = processo->primFilho;
        while(p){
            free(p);
            p = p->proxIrmao;
        }
        //Exclui o processo
        free(processo);
        return 1;
    }
    return 0;
}


//2 - Quando processos que tem filhos sao excluidos, os filhos devem ser ligados ao vertice superior - avo
int exclusaoTipoDois(int px, no raiz){
    no processo = buscarProcesso(raiz, px);
    //Verifica se o processo existe
    if(processo){
        //Pega os filhos do processo
        no filhos = processo->primFilho;
        //Procurar o pai do processo
        no avo = processo->pai;
        //Pega o primeiro filho do avo
        no aux = avo->primFilho;

        if(aux->chave==px && aux->proxIrmao==NULL){
            //Filho unico
            (*avo).primFilho = filhos;
            free(processo);
            return 1;
        }
        else if(aux->chave==px && aux->proxIrmao!=NULL){
            //Filho mais novo
            (*avo).primFilho = processo->proxIrmao;
        }
        else{
            //Nao é o filho mais novo
            no irmaoNovo = aux;

            //Exclui o processo da lista de filhos do pai dele
            while(aux){
                //Precisa pegar o irmao anterior
                if(aux->chave==px){
                    (*irmaoNovo).proxIrmao = aux->proxIrmao;
                    break;
                }
                irmaoNovo = aux;
                aux = aux->proxIrmao;
            }
        }
        //Finaliza a exclusao
        aux = avo->primFilho;

        while(aux){
            //Busca o filho mais velho do avo
            if(aux->proxIrmao==NULL){
                break;
            }
            aux = aux->proxIrmao;
        }
        //Como eu faco pra pegar o ultimo processo? Aquele que não está vazio

        //Adiciona os processos filhos do processo no avo
        (*aux).proxIrmao = filhos;

        //Exclui o processo
        free(processo);
        return 1;
    }
    return 0;
}


void escolherPolitica(){
    while(1){
        printf("\nPOLITICAS DE EXCLUSAO DISPONIVEIS:\n");
        printf("0 - Processos que tem filhos nao podem ser excluidos\n");
        printf("1 - Quando processos que tem filhos sao excluidos, toda a subarvore a partir do ponto de exclusao deve ser excluida\n");
        printf("2 - Quando processos que tem filhos sao excluidos, os filhos devem ser ligados ao vertice superior - avo\n");
        printf("\nDigite o numero da politica de exclusao desejada:");
        scanf("%d", &politicaExclusao);
        if(!(politicaExclusao==0 || politicaExclusao==1 || politicaExclusao==2)){
            printf("\nOpcao invalida! Digite uma opcao valida para continuar.\n");
        }
        else{
            break;
        }
    }
}

int main(){
    //Raiz da arvore
    no raiz;
    //p - pai, f - filho
    int pai, filho, processo, resultado;
    char parametro[6];
	char p1[6],p2[6];
	char *token;

    //Escolher politica
    escolherPolitica();
    printf("\n\nPolitica escolhida: %d\n", politicaExclusao);

    while(1){
        int opcao;
        printf("\n\nMENU\n");
        if(processoInicial==-1){
            printf("1 - Criar processo inicial\n");
            printf("2 - Sair\n");
            printf("-Para ter acesso as outras opcoes do sistema, crie o processo inicial-");
            printf("\n\nDigite o numero da opcao desejada:");
            scanf("%d", &opcao);
            switch(opcao){
                case 1:
                    //Funcao para criar o processo inicial
                    processoInicial = 1;
                    raiz = criarProcessoInicial(0);
                    printf("\nChave do processo inicial: 0\n");
                    break;
                case 2:
                    printf("Obrigada!");
                    break;
                default:
                   printf("\nOpcao invalida! Digite uma opcao valida para continuar.\n");
                   break;
            }
            if(opcao==2){
                break;
            }
        }
        else{
            printf("1 - Criar novo processo\n");
            printf("2 - Destruir um processo\n");
            printf("3 - Contar a quantidade de processos abertos\n");
            printf("4 - Visualizar a arvore de relacionamento entre processos\n");
            printf("5 - Sair\n");
            printf("\nDigite o numero da opcao desejada:");
            scanf("%d", &opcao);
            switch(opcao){
                case 1:
                    printf("Digite as informacoes do processo: ");
                    //Recebe as informações do processo
                    scanf("%s", parametro);

                    //Trata o parametro
                    //Separa a string quando acha o traço
                    //Pega a primeira parte da string
                    token = strtok(parametro, "-");
                    //Passa a string para p1
                    strcpy(p1,token);
                    //Pega a partir do segundo caracter da string
                    strcpy(p1,&p1[1]);
                    //Pega a segunda parte da string
                    token = strtok(NULL, " ");
                    strcpy(p2,token);
                    strcpy(p2,&p2[1]);

                    pai = atoi(p1);
                    filho = atoi(p2);

                    if((inserirProcesso(pai,filho,raiz))==0){
                        printf("\nPai escolhido nao existe");
                    }else{
                        printf("Processo adicionado com sucesso!");
                    }
                    break;
                case 2:
                    //Excluir processo

                    printf("Digite as informacoes do processo: ");
                    //Recebe as informações do processo
                    scanf("%s", parametro);

                    //Trata a string
                    //Pega a partir do segundo caracter da string
                    strcpy(parametro,&parametro[1]);
                    //Transforma a string restante em inteiro
                    processo = atoi(parametro);

                    if(processo==0){
                        printf("O processo inicial nao pode ser excluido!\n");
                    }else{
                        if(politicaExclusao==0){
                            resultado = exclusaoTipoZero(processo, raiz);
                            if(resultado==1){
                                printf("Processo excluido com sucesso!");
                            }else if(resultado==2){
                                printf("\nA politica de exclusao nao permite a exclusao desse processo!");
                            }
                            else{
                                printf("O processo nao existe!");
                            }
                        }else if(politicaExclusao==1){
                            resultado = exclusaoTipoUm(processo,raiz);
                            if(resultado==1){
                                printf("Processo excluido com sucesso!");
                            }else{
                                printf("Processo nao existe");
                            }
                        }else{
                            resultado = exclusaoTipoDois(processo,raiz);
                            if(resultado==1){
                                printf("Processo excluido com sucesso!");
                            }else{
                                printf("Processo nao existe");
                            }
                        }
                    }
                    break;
                case 3:
                    printf("\nQuantidade de processos existentes: %d",quantidadeProcessos(raiz));
                    break;
                case 4:
                    printf("\nPROCESSOS EXISTENTES: ");
                    imprimirProcessos(raiz);
                    printf("\n");
                    break;
                case 5:
                    printf("Obrigada!");
                    break;
                default:
                   printf("\nOpcao invalida! Digite uma opcao valida para continuar.\n");
                   break;
            }
            if(opcao==5){
                break;
            }
        }
    }

    return 0;
}
