#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

struct jogador //Struct serve para grupar variáveis.
{
    int vida;           
    int dica_jogo;      
    int trocar_questao; 
    int pular_nivel;
    int acertou_muito_facil;
    int acertou_muito_facil2;
    int acertou_facil;
    int acertou_facil2;
    int acertou_medio;
    int acertou_medio2;
    int acertou_dificil;
    int acertou_dificil2;
    int acertou_muito_dificil;
    int acertou_muito_dificil2;
};

struct questoes
{
    char pergunta[1500];
    char A[1500];
    char B[1500];
    char C[1500];
    char D[5000];
    char resposta[2];
    char dica[1500];
};

// Função para pegar pergunta aleatória do JSON usando strstr
struct questoes pegarPerguntaAleatoria(char *jsonTexto, const char *categoria)
{
    struct questoes jogo;                      // struct local para armazenar a pergunta.
    memset(&jogo, 0, sizeof(struct questoes)); // inicializa com zeros

    char chave[50];
    snprintf(chave, sizeof(chave), "\"%s\"", categoria);

    char *inicio = strstr(jsonTexto, chave);
    if (!inicio)
    {
        printf("Categoria %s não encontrada.\n", categoria);
        return jogo; // retorna struct vazia
    }

    char *arrayStart = strchr(inicio, '[');
    if (!arrayStart)
    {
        printf("Array não encontrado.\n");
        return jogo;
    }

    // Contar perguntas
    int totalPerguntas = 0;
    char *p = arrayStart;
    char *arrayEnd = strchr(arrayStart, ']');
    while ((p = strchr(p, '{')) != NULL && p < arrayEnd)
    {
        totalPerguntas++;
        p++;
    }

    if (totalPerguntas == 0)
    {
        printf("Nenhuma pergunta encontrada.\n");
        return jogo;
    }

    // Escolher índice aleatório
    int indice = rand() % totalPerguntas;

    // Pular para a pergunta escolhida
    p = arrayStart;
    for (int i = 0; i <= indice; i++)
    {
        p = strchr(p, '{');
        if (!p)
        {
            printf("Erro ao encontrar pergunta.\n");
            return jogo;
        }
        p++;
    }

    // Extrair campos
    sscanf(strstr(p, "\"pergunta\""), "\"pergunta\": \"%[^\"]\"", jogo.pergunta);
    sscanf(strstr(p, "\"A\""), "\"A\": \"%[^\"]\"", jogo.A);
    sscanf(strstr(p, "\"B\""), "\"B\": \"%[^\"]\"", jogo.B);
    sscanf(strstr(p, "\"C\""), "\"C\": \"%[^\"]\"", jogo.C);
    sscanf(strstr(p, "\"D\""), "\"D\": \"%[^\"]\"", jogo.D);
    sscanf(strstr(p, "\"resposta\""), "\"resposta\": \"%1[^\"]\"", jogo.resposta);
    sscanf(strstr(p, "\"dica\""), "\"dica\": \"%[^\"]\"", jogo.dica);

    return jogo;
}

void regras()
{
    int c;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para suportar cores.
    SetConsoleOutputCP(CP_UTF8);                // No Windows, habilitar UTF-8 no Terminal do Windows.

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("==== As regras são: ====\n\n");

    while ((c = getchar()) != '\n' && c != EOF)
        ;

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Branco
    printf(" 1.O jogador inicia o jogo com 1 vida. Ao responder uma questão errada, o jogador perde 1 vida.\n\n 2.Se a vida chegar a 0, o jogador perde o jogo e volta para o início.\n\n 3.Ao responder uma questão corretamente, o jogador passa para o próximo nível de dificuldade.\n O nível de dificuldade aumenta a cada pergunta respondida corretamente.\n\n 4.O jogador vence o jogo se responder corretamente as 10 perguntas do último nível de dificuldade.\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("Responda todas as perguntas a seguir e passe de ano. A cada pergunta errada você perde 1 ponto. 🩵\n\n");
    Sleep(1000);

    while ((c = getchar()) != '\n' && c != EOF)
        ;
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Branco
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("====  Ações especiais:  ====\n\n");

    while ((c = getchar()) != '\n' && c != EOF)
        ;

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Branco
    printf("O jogador poderá usar cada uma das seguintes ações especiais uma única vez por partida: 😍\n\n 1.Pular Questão: O jogador passa diretamente para o próximo nível, sem responder a pergunta do nível atual.\n\n 2.Trocar Questão: O jogo sorteia outra pergunta do nível atual para que o jogador responda.\n\n 3.Dica: O jogo apresenta uma dica para a resposta da questão atual.\n\n");
    // while ((c = getchar()) != '\n' && c != EOF);
}

void menu_escolha()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para suportar cores.
    SetConsoleOutputCP(CP_UTF8);                // No Windows, habilitar UTF-8 no Terminal do Windows.

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("======== Menu =======\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("1. Responder.\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("2. Pular Nível.\n");
    printf("3. Trocar questão.\n");
    printf("4. Dica.\n");
    printf("5. Voltar ao Menu.\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("6. Sair do jogo.\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("7. Ver Status do jogo.\n\n");

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("Selecione uma opção: ");
}

void esperarEnter()
{
    // Limpa qualquer lixo que esteja no buffer
    fflush(stdin);
    getchar(); // espera o Enter real
}

void sair_jogo()
{ // Funçaõ de encerrar o jogo quando soliticado.
    printf("Saindo do jogo...\n");
    Sleep(2000);
    exit(0); // termina imediatamente
}



void mostrarDica(struct questoes *jogo, struct jogador *player, char *nivelParte)
{
    int *dicaDisponivel = NULL;

    // Determina qual variável de dica do jogador usar
    if (strcmp(nivelParte, "muito_facil") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "muito_facil2") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "facil") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "facil2") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "medio") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "medio2") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "dificil") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "dificil2") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "muito_dificil") == 0)
        dicaDisponivel = &player->dica_jogo;
    else if (strcmp(nivelParte, "muito_dificil2") == 0)
        dicaDisponivel = &player->dica_jogo;

    if (dicaDisponivel && *dicaDisponivel == 1)
{
    system("cls");
    printf("Sua dica é:\n%s\n\n", jogo->dica);
    *dicaDisponivel = 0; // Agora sim marca como usada
    Sleep(2000);
}
else
{
    system("cls");
    printf("Você já utilizou a dica nesse nível...\n");
    Sleep(2000);
}
}


void mostrarStatus(struct jogador player, char *nivelParte)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para suportar cores.
    SetConsoleOutputCP(CP_UTF8);                // No Windows, habilitar UTF-8 no Terminal do Windows.
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("===== Status do jogo =====\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf("Vida: %d pts\n", player.vida);
    printf("Trocar questão: %d pts\n", player.trocar_questao);
    printf("Pular nível: %d pts\n", player.pular_nivel);
    printf("Dica: %d pts\n", player.dica_jogo);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\n========  Nível Atual: %s ========\n ", nivelParte);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\nPressione Enter para continuar...\n");
    
}

// Função para ler arquivo
char *lerArquivo(const char *dados)
{
    FILE *f = fopen(dados, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *texto = malloc(tamanho + 1);
    fread(texto, 1, tamanho, f);
    texto[tamanho] = '\0';
    fclose(f);
    return texto;
}

void trocarQuestao(char *jsonTexto, char *categoria, struct questoes *jogo, struct jogador *player, char *nivelParte)
{
    if (player->trocar_questao == 1)
    {
        player->trocar_questao = 0;
        printf("Trocando questão...Existe uma chance de cair a mesma pergunta...\n");
        Sleep(2000);

        // Pega nova pergunta aleatória
        pegarPerguntaAleatoria(jsonTexto, categoria);

        // Reseta a flag do nível correspondente para permitir nova resposta
        if (strcmp(nivelParte, "muito_facil") == 0)
            player->acertou_muito_facil = 0;
        else if (strcmp(nivelParte, "muito_facil2") == 0)
            player->acertou_muito_facil2 = 0;
        else if (strcmp(nivelParte, "facil") == 0)
            player->acertou_facil = 0;
        else if (strcmp(nivelParte, "facil2") == 0)
            player->acertou_facil2 = 0;
        else if (strcmp(nivelParte, "medio") == 0)
            player->acertou_medio = 0;
        else if (strcmp(nivelParte, "medio2") == 0)
            player->acertou_medio2 = 0;
        else if (strcmp(nivelParte, "dificil") == 0)
            player->acertou_dificil = 0;
        else if (strcmp(nivelParte, "dificil2") == 0)
            player->acertou_dificil2 = 0;
        else if (strcmp(nivelParte, "muito_dificil") == 0)
            player->acertou_muito_dificil = 0;
        else if (strcmp(nivelParte, "muito_dificil2") == 0)
            player->acertou_muito_dificil2 = 0;
    }
    else
    {
        printf("Você já usou a troca!\n");
        Sleep(2000);
    }
}

// Função para responder pergunta (retorna 1 se acertou, 0 se errou)
int responderPergunta(struct questoes *jogo, struct jogador *player, char *resposta_jogador, HANDLE h, char *nivelParte)
{
    while (getchar() != '\n')
        ; // limpar buffer
    printf("Sua resposta: ");
    scanf("%c", &resposta_jogador[0]);
    resposta_jogador[0] = toupper(resposta_jogador[0]);

    if (strcmp(resposta_jogador, jogo->resposta) == 0)
    {
        system("cls");
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("Parabéns! Você acertou.\n");
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        Sleep(4000);

        // Atualiza a variável certa do jogador
        if (strcmp(nivelParte, "muito_facil") == 0)
            player->acertou_muito_facil = 1;
        else if (strcmp(nivelParte, "muito_facil2") == 0)
            player->acertou_muito_facil2 = 1;
        else if (strcmp(nivelParte, "facil") == 0)
            player->acertou_facil = 1;
        else if (strcmp(nivelParte, "facil2") == 0)
            player->acertou_facil2 = 1;
        else if (strcmp(nivelParte, "medio") == 0)
            player->acertou_medio = 1;
        else if (strcmp(nivelParte, "medio2") == 0)
            player->acertou_medio2 = 1;
        else if (strcmp(nivelParte, "dificil") == 0)
            player->acertou_dificil = 1;
        else if (strcmp(nivelParte, "dificil2") == 0)
            player->acertou_dificil2 = 1;
        else if (strcmp(nivelParte, "muito_dificil") == 0)
            player->acertou_muito_dificil = 1;
        else if (strcmp(nivelParte, "muito_dificil2") == 0)
            player->acertou_muito_dificil2 = 1;

        return 1; // acertou
    }
    else
    {
        system("cls");
        printf("Errado! A resposta correta era %s.\n", jogo->resposta);
        printf("Você perdeu!\n");
        Sleep(4000);
        system("cls");
        printf("Pressione Enter para voltar ao Menu Principal....\n");
        esperarEnter();
        return 0; // errou
    }
}


   void pularNivel(struct jogador *player, int *sair_do_menu, char *nivelParte)
{
    system("cls");

    if (player->pular_nivel > 0)
    {
        printf("Pulando o nível: %s...\n", nivelParte);
        Sleep(2000);

        player->pular_nivel--;  // Usa o pulo
        *sair_do_menu = 1;      // Sai do menu do nível
    }
    else
    {
        printf("Você já usou seu pulo nesse jogo. Não é possível pular outro nível.\n");
        Sleep(2500);
    }
}



int main()
{

    char *jsonTexto = lerArquivo("dados.json");
    if (!jsonTexto)
    {
        printf("Erro ao ler o arquivo JSON.\n");
        return 1;
    }

    // Início da apresentação jogo.

    srand(time(NULL)); // Inicializado de números aleatórios para a questão.
    int opcao_sair_iniciar;
    int c;

    // Área para definir outras funções =======================================================
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para suportar cores.
    SetConsoleOutputCP(CP_UTF8);                // No Windows, habilitar UTF-8 no Terminal do Windows.

    //===========================================================================================

    // Apresentação do contexto/jogo.

    puts("Olá, estudante! Seja bem-vindo ao: Sobreviva à Universidade.📚\n");
    Sleep(1000);
    SetConsoleTextAttribute(h, FOREGROUND_INTENSITY); // cinza fraco
    puts("Pressione Enter para Continuar...");

    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Volta para Branco. Não se esquece que cada vez tem que voltar para branco.

    esperarEnter(); // espera o usuário apertar Enter//BOTÃO

    printf("Você, aparentemente, não foi um aluno dedicado esse semestre.\n\nEntão, o professor de Algoritmo e Programação Estruturada te deu chance de recuperar suas notas.\n\nEle te passou, em sala, dez perguntas para serem respondidas corretamente.\n\nAs perguntas são relacionadas ao seu curso de Engenharia de Software.\n\nReponda todas as 10 perguntas e passe para o próximo semestre!😃\n\n"); // Apresentação Inicial.

    Sleep(1000);
    SetConsoleTextAttribute(h, FOREGROUND_INTENSITY); // cinza fraco.
    // Botão Enter. (2) Apresentação (2)
    puts("Pressione Enter para Continuar...");
    esperarEnter();

    regras(); // Apresentação das regras.

    Sleep(1000);

    SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
    puts("Pressione Enter para Continuar...");

    Sleep(1000);
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    esperarEnter();
    system("cls"); // Fim da Apresentação.

    // Menu Do jogo. =============================

    puts("Vamos Começar?\n");
    Sleep(2000);

menu:
    do
    { // Funções interativas Menu do jogo.

        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n--- MENU ---\n\n");
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("1. Jogar\n"); // Jogar
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("2. Sair\n");                                                                                    // Sair
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Volta para Branco. Não se esquece que cada vez tem que voltar para branco.
        printf("3. (Rever Regras do Jogo)\n\n");                                                                // Ver Regras do Jogo.
        SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);                                                       // cinza fraco
        printf("Selecione uma opção: ");
        fflush(stdin);
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        fflush(stdin);

        // Seleção do Jogador.

        scanf("%d", &opcao_sair_iniciar);
        while (getchar() != '\n')
            ;

        switch (opcao_sair_iniciar)
        {
        case 1:
            printf("\n"); // Para pular linhas.
            printf("Iniciando o jogo...\n");
            Sleep(2000);
            break;
        case 2:
            Sleep(2000);
            sair_jogo(); // Para Fechar o programa. ======== SAIR DO JOGO =======
        case 3:
            system("cls");
            regras();
            Sleep(2000);
            printf("\n");                                     // Para pular linhas.
            SetConsoleTextAttribute(h, FOREGROUND_INTENSITY); // cinza fraco
            printf("Pressione Enter para voltar ao menu...");
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Volta para Branco. Não se esquece que cada vez tem que voltar para branco.
            esperarEnter();                                                                                         // Entender porque tive que colocar dois getchar? Tem algo a ver com o scanf anterior?
            Sleep(2000);
            printf("\n"); // Para pular linhas.
            ("Voltando ao Menu principal...");
            system("cls"); // limpa a tela antes de voltar ao menu
            break;
        default: // Selecionou outros botões.
            printf("Opção inválida!\n");
            Sleep(2000);
            system("cls");
            printf("Escolha uma opção válida.");
            break;
        } // Fecha Switch.
    } while (opcao_sair_iniciar != 1); // repete até o usuário escolher 1(Depois de Escolher 1, ele vai para o jogo.) // Fecha Do.

    system("cls");
    Sleep(1000);

    struct jogador player = {  // Serve para manter informações relacionadas juntas, como se fosse uma ficha ou um registro.
        .vida = 1,
        .dica_jogo = 1,
        .trocar_questao = 1,
        .pular_nivel = 1,
        .acertou_muito_facil = 0,
        .acertou_muito_facil2 = 0,
        .acertou_facil = 0,
        .acertou_facil2 = 0,
        .acertou_medio = 0,
        .acertou_medio2 = 0,
        .acertou_dificil = 0,
        .acertou_dificil2 = 0,
        .acertou_muito_dificil = 0,
        .acertou_muito_dificil2 = 0,
    };

    struct questoes jogo = {
        .pergunta = "",
        .A = "",
        .B = "",
        .C = "",
        .D = "",
        .resposta = "",
        .dica = ""};

    //    ================ Início do Jogo =========================

    printf("JOGO --> Sobreviva à universidade.\n\n");

    //==================== Nível muito fácil =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível muito fácil ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // Ler o arquivo JSON apenas uma vez

   while (!player.acertou_muito_facil)
    {
        char resposta_jogador[2] = "";

        jogo = pegarPerguntaAleatoria(jsonTexto, "muito_facil");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "muito_facil"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;
            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "muito_facil");

                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "muito_facil", &jogo, &player, "muito_facil");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "muito_facil");
                break;
            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Muito Facil");
                esperarEnter();

                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....\n");
    Sleep(2000);

    //==================== Nível muito fácil - Parte 2 =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível muito fácil - Parte 2 ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // Ler o arquivo JSON apenas uma vez

   while (!player.acertou_muito_facil2)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "muito_facil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "muito_facil2");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "muito_facil2"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "muito_facil2");
                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "muito_facil", &jogo, &player, "muito_facil2");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "muito_facil2");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Muito Facil - Parte 2");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível fácil =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível fácil ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // Ler o arquivo JSON apenas uma vez

   while (!player.acertou_facil)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "facil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "facil");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "facil"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "facil");

                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "facil", &jogo, &player, "facil");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "facil");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Facil");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }
    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível fácil - Parte 2 =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível fácil - Parte 2 ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // Ler o arquivo JSON apenas uma vez

   while (!player.acertou_facil2)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "facil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "facil2");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "facil2"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível 
                system("cls");
                pularNivel(&player, &sair_do_menu, "facil2");

                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "facil2", &jogo, &player, "facil2");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "facil2");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Facil - Parte 2");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível médio =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível médio ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    while (!player.acertou_medio)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "medio"
        jogo = pegarPerguntaAleatoria(jsonTexto, "medio");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "medio"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "medio");

                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "medio", &jogo, &player, "medio");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "medio");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Medio");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível médio 2 =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível médio - Parte 2 ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    while (!player.acertou_medio2)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "medio"
        jogo = pegarPerguntaAleatoria(jsonTexto, "medio2");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "medio2"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "medio2");
                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "medio", &jogo, &player, "medio2");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "medio2");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "Medio - Parte 2");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível difícil =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível difícil ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

     while (!player.acertou_dificil)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "dificil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "dificil");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1:
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "dificil"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2:
                system("cls");
                pularNivel(&player, &sair_do_menu, "dificil");

                break;

            case 3:
                system("cls");
                trocarQuestao(jsonTexto, "dificil", &jogo, &player, "dificil");
                break;

            case 4:
                mostrarDica(&jogo, &player, "dificil");
                break;

            case 5:
                system("cls");
                goto menu;
                break;

            case 6:
                system("cls");
                sair_jogo();
                break;

            case 7:
                system("cls");
                mostrarStatus(player, "Dificil");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível difícil 2 =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível difícil - Parte 2 ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    while (!player.acertou_dificil2)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "dificil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "dificil2");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "dificil2"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "dificil2");
                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "dificil2", &jogo, &player, "dificil2");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "dificil2");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player,"Dificil - Parte 2");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....");
    Sleep(2000);
    system("cls");

    //==================== Nível muito difícil =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível muito difícil ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

     while (!player.acertou_muito_dificil)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "muito_dificil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "muito_dificil");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1:
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "muito_dificil"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2:
                system("cls");
                pularNivel(&player, &sair_do_menu, "muito_dificil");

                break;

            case 3:
                system("cls");
                trocarQuestao(jsonTexto, "muito_dificil", &jogo, &player, "muito_dificil");
                break;

            case 4:
                mostrarDica(&jogo, &player, "muito_dificil");
                break;

            case 5:
                system("cls");
                goto menu;
                break;

            case 6:
                system("cls");
                sair_jogo();
                break;

            case 7:
                system("cls");
                mostrarStatus(player, "Muito Dificil");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Vamos para o próximo nível....\n");
    Sleep(2000);

    //==================== Nível muito difícil 2 =====================
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("======== Nível muito difícil - Parte 2 ========\n\n");
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

     while (!player.acertou_muito_dificil2)
    {
        char resposta_jogador[2] = "";

        // Pega uma pergunta aleatória da categoria "muito_dificil"
        jogo = pegarPerguntaAleatoria(jsonTexto, "muito_dificil2");

        int sair_do_menu = 0;

        while (!sair_do_menu)
        {
            printf("Pergunta:\n\n%s\n\n", jogo.pergunta);
            printf("A: %s\nB: %s\nC: %s\nD: %s\n\n\n", jogo.A, jogo.B, jogo.C, jogo.D);
            Sleep(2000);

            menu_escolha();
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1: // Responder pergunta
                if (responderPergunta(&jogo, &player, resposta_jogador, h, "muito_dificil2"))
                {
                    sair_do_menu = 1;
                }
                else
                {
                    goto menu;
                }
                break;

            case 2: // Pular nível
                system("cls");
                pularNivel(&player, &sair_do_menu, "Muito Dificil - Parte 2");
                break;

            case 3: // Trocar questão
                system("cls");
                trocarQuestao(jsonTexto, "muito_dificil", &jogo, &player, "muito_dificil2");
                break;

            case 4: // Dica
                mostrarDica(&jogo, &player, "muito_dificil2");
                break;

            case 5: // Voltar ao Menu
                system("cls");
                goto menu;
                break;

            case 6: // Sair do jogo
                system("cls");
                sair_jogo();
                break;

            case 7: // Status do jogo
                system("cls");
                mostrarStatus(player, "muito_dificil2");
                esperarEnter();
                break;

            default:
                printf("Opção inválida! Selecione novamente.\n");
                Sleep(1000);
                break;
            }
        } // Fim do menu do nível
        if (sair_do_menu) break;
    }

    system("cls");
    puts("Parabéns! Você completou o jogo!");
    Sleep(3000);
    system("cls");

    printf("🩵🩵🩵🩵Parabéns!, Você acertou todas as perguntas. O professor te passou de ano e você pode dormir em paz!🩵🩵🩵🩵\n");
    Sleep(3000);
    printf("\nPressione Enter para voltar ao Menu Principal.....");
    Sleep(1000);
    fflush(stdin);
    getchar();
    system("cls");
    goto menu;

} // Final Código.
