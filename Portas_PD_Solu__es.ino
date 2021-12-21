#include<avr/pgmspace.h>

int la1 = 3;      //porta digital onde o led amarelo da porta 1 está conectado
int la2 = 2;      //porta digital onde o led amarelo da porta 2 está conectado
int lvd1 = 5;     //porta digital onde o led verde da porta 1 está conectado
int lvd2 = 4;     //porta digital onde o led verde da porta 2 está conectado
int lvm1 = 7;     //porta digital onde o led vermelho da porta 1 está conectado
int lvm2 = 6;     //porta digital onde o led vermelho da porta 2 está conectado
int bzz = 9;      //porta digital onde o buzzer está conectado
int bt1 = 10;     //porta digital onde o botão para fechamento da porta 1 está conectado
int bt2 = 11;     //porta digital onde o botão para fechamento da porta 1 está conectado

unsigned long time;

int  k = -1, n = 0, s = 0, a = 0, i = -1,w = 0, vf, p, z = 0, acporta[20], f;       //variáveis para o controle de loop e autenticação de funções
char menu10 = '1';
String nm, sn,nome[20],senha[20],adm[20],acnome[20],ad;                             //strings para o reconhecimento de nomes provisórios e vetores de strings permanentes
const char ksc[3] PROGMEM = "nu";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                                               
  while(!Serial);                                 //espera a porta serial se conectar
  pinMode(la1, OUTPUT);                           //início da decraração do regime de operação das portas
  pinMode(la2, OUTPUT);
  pinMode(lvd1, OUTPUT);
  pinMode(lvd2, OUTPUT);
  pinMode(lvm1, OUTPUT);
  pinMode(lvm2, OUTPUT);
  pinMode(bzz, OUTPUT);
  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);                     //fim da decraração do regime de operação das portas

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if((k == -1)&&((menu10 == '1') || (menu10 == '2') || (menu10 == '3') || (menu10 == '4') || (menu10 == '5'))){  //k == -1 é o fator que autentica a leitura do menu principal
  Serial.println(" ");                                                                                           //início da impressão do menu principal no monitor serial
  Serial.println("#############################################");
  Serial.println("#########Acesso às salas PD Soluções#########");
  Serial.println("#####(digite o número da opção desejada)#####");
  Serial.println("1.Liberação da Sala 1");
  Serial.println("2.Liberação da Sala 2");
  Serial.println("3.Cadastro de Usuário");
  Serial.println("4.Listagem dos Usuários Cadastrados");
  Serial.println("5.Listagem dos Eventos");
  Serial.println(" ");                                                                                           //fim da impressão do menu principal no monitor serial
    leituraMenu();                                                                                               //chama a função responsável pela leitura das escolhas do menu principal
  }
  
  menu10 = leituraMenu();                                                                                        //iguala a variável de leitura com o caractere retornado da função
  if((menu10 == '1') || (menu10 == '2') || (menu10 == '3') || (menu10 == '4') || (menu10 == '5')){

  if(menu10 == '3'){                                                                              //OPÇÃO PARA O CADASTRO DE USUÁRIOS
    
    
    if(n == -1){                                                                                  //n == -1, s == -1 e a == -1 são variáveis que autenticam a execução das funções
      cadastroNome();                                                                             //chama a função para o cadastro do nome de um novo usuário
    }
    if(s == -1){
      cadastroSenha();                                                                            //chama a função para o cadastro da senha de um novo usuário
    }
    if(a == -1){
      cadastroAdm();                                                                              //chama a função que dá o status ou não de administrador para um novo usuário
    }
    
    if(k == 12){
      
      
      nome[i] = nm;                                                                               //atribui às variáveis permanentes(vetores) os valores temporários retornados pelas funções
      senha[i] = sn;
      adm[i] = ad;

      Serial.println("--->CADASTRO REALIZADO COM SUCESSO<---");
      delay(1000);
      k = -1;
    }
  }

  if(menu10 == '4'){                                                                              //OPÇÃO PARA A LISTAGEM DE USUÁRIOS JÁ CADASTRADOS
    
 
    for(w = 0;w < 20; w++){                                                                       //Percorre os vetores de nome e status de administrador e imprime apenas os existentes
      
      if((adm[w] == "0")||(adm[w] == "1")){
        Serial.print("ID: ");
        Serial.println(w);
        Serial.print("Nome: ");
        Serial.println(nome[w]);
        if(adm[w] == "1"){
          Serial.println("Administrador");
        }
        else if(adm[w] == "0"){
          Serial.println("Sem permissões especiais");
        }
      }
    }
    k = -1;
  }

  if(menu10 == '1'){                                                                               //OPÇÃO DE ABERTURA DA PORTA DA SALA 1 
    digitalWrite(la1, HIGH);
    n = -1;
    vf = -1;
    leituraNome();                                                                                 //chama a função para a leitura temporária de nome e senha
    leituraSenha();

    for(p = 0; p <= 20; p++){
      if(nm == nome[p]){
        vf = 0;                                                                                    //verifica que o nome de usuário de fato existe 
        if(sn == senha[p]){
          time = millis();
         
          digitalWrite(lvd1, HIGH);                                                                //ação análoga a abertura da porta 1
          Serial.println("--->SALA 1 ABERTA<---");
          tone(bzz, 1000);
          while((millis() - time <= 1000)&&(digitalRead(bt1) == 1));                               //as funções de timer são divididas em dois porque o buzzer deve tocar por apenas 1s
          noTone(bzz);
          while((millis() - time <= 4000)&&(digitalRead(bt1) == 1));
          digitalWrite(lvm1, HIGH);                                                                //ação análoga ao fechamento da porta
          delay(400);
          acnome[z] = nome[p];                                                                     //registra o nome do usuário que abriu a porta para futuras verificações
          acporta[z] = 1;                                                                          //registra qual porta foi aberta
          z++;
        }
        else{                                                                                      //sinaliza com um print, aviso sonoro e luminoso que a senha está incorreta
          digitalWrite(lvm1, HIGH);
          Serial.println("--->SENHA INCORRETA<---");
          tone(bzz, 200);
          delay(1000);
          noTone(bzz);
          
        }
      }
    }
    if(vf == -1){
      Serial.println("--->USUÁRIO NÃO ENCONTRADO<---");
    }
    k = -1;                                                                                        //permite que o menu principal seja executado novamente
    digitalWrite(la1, LOW);
    digitalWrite(lvd1, LOW);
    digitalWrite(lvm1, LOW);
  }

  if(menu10 == '2'){                                                                               //OPÇÃO DE ABERTURA DA PORTA DA SALA 2
    digitalWrite(la2, HIGH);
    n = -1;
    vf = -1;
    leituraNome();                                                                                 //chama a função para a leitura temporária de nome e senha
    leituraSenha();

    for(p = 0; p <= 20; p++){
      if(nm == nome[p]){
        vf = 0;                                                                                    //verifica que o nome de usuário de fato existe 
        if(sn == senha[p]){
          time = millis();
         
          digitalWrite(lvd2, HIGH);                                                                //ação análoga a abertura da porta 2
          Serial.println("--->SALA 2 ABERTA<---");
          tone(bzz, 1000);
          while((millis() - time <= 1000)&&(digitalRead(bt2) == 1));                               //as funções de timer são divididas em dois porque o buzzer deve tocar por apenas 1s
          noTone(bzz);
          while((millis() - time <= 4000)&&(digitalRead(bt2) == 1));
          digitalWrite(lvm2, HIGH);                                                                //ação análoga ao fechamento da porta
          delay(400);
          acnome[z] = nome[p];                                                                     //registra o nome do usuário que abriu a porta para futuras verificações
          acporta[z] = 2;                                                                          //registra qual porta foi aberta
          z++;
        }
        else{                                                                                      //sinaliza com um print, aviso sonoro e luminoso que a senha está incorreta
          digitalWrite(lvm2, HIGH);
          Serial.println("--->SENHA INCORRETA<---");
          tone(bzz, 200);
          delay(1000);
          noTone(bzz);
          
        }
      }
    }
    if(vf == -1){
      Serial.println("--->USUÁRIO NÃO ENCONTRADO<---");
    }
    k = -1;                                                                                        //permite que o menu principal seja executado novamente
    digitalWrite(la2, LOW);
    digitalWrite(lvd2, LOW);
    digitalWrite(lvm2, LOW);
  }

  if(menu10 == '5'){                                                                      //OPÇÃO PARA A LISTAGEM DOS EVENTOS DE ABERTURA DAS PORTAS
    n = -1;
    leituraNome();                                                                        //chama a função para a leitura temporária de nome e senha
    leituraSenha();
    for(f = 0; f <= 20; f++){                                                             //verifica se nome e senha são compatíveis aos salvos no banco de dados
      if(nm == nome[f]){
        if(sn == senha[f]){
          if(adm[f] == "1"){                                                              //verifica se o usuário é um administrador, em caso positivo imprime as informações
            for(f = 0; f < z; f++){
            Serial.print("Usuário: ");
            Serial.println(acnome[f]);
            Serial.print("Sala aberta: ");
            Serial.println(acporta[f]);
            Serial.println(" ");
    }
          }
          else{
            Serial.println("--->O USUÁRIO NÃO É UM ADMINISTRADOR<---");
          }
        }
        else{
          Serial.println("--->SENHA INCORRETA<---");
        }
      }
      
    }
    
    k = -1;                                                                                //permite que o menu principal seja executado novamente
  }
  
  }
  else{
    k = -1;                                                                 //se uma opção não disponível for lida pela serial o menu principal é executado novamente
  }
}
char leituraMenu(){                                     //FUNÇÃO DE LEITURA DA OPÇÃO ESCOLHIDA NO MENU PRINCIPAL
  while(k == -1){
 
  if(Serial.available() >= 1){
    menu10 = Serial.read();
    k = 0;
    n = -1;                                             //autoriza que a função de cadastro de nomes ou leitura de nomes seja executada
    
  }
  }
  
  return menu10;
}

String cadastroNome(){                                                  //FUNÇÃO PARA O CADASTRO DO NOME DE UM NOVO USUÁRIO
  if(n == -1){    
    Serial.println("Digite seu nome: ");
    n = 0;    
  }  
  while(k == 0){    
  while(Serial.available() > 1){    
   nm = Serial.readString();
   nm.trim();
   Serial.println(nm);
   i++;   
    s = -1;                                                             //autoriza que a função de cadastro de senha ou leitura de senha seja executada
    n = 1;                                                              //autoriza que a própria função retorne o nome
  }  
  if(n == 1){
  return nm;
  }
  }  
}

String cadastroSenha(){                                                 //FUNÇÃO PARA O CADASTRO DE SENHA DE UM NOVO USUÁRIO
  if(s == -1){        
    Serial.println("Digite sua senha: ");
    s = 0;   
  }
  while(k == 0){  
  while(Serial.available() > 1){    
   sn = Serial.readString();
   sn.trim();
    Serial.println(sn);
    a = -1;                                                             //autoriza que a função para a atribuição do status de administrador seja executada 
    s = 1;                                                              //autoriza que a própria função retorne a senha
  }
  if(s == 1){
    return sn;
  }
  }  
}

String cadastroAdm(){                                                   //FUNÇÃO PARA A ATRIBUIÇÃO DE PERMISSÕES ESPECIAIS AO NOVO USUÁRIO(ADMINISTRADOR OU NÃO)
  if(a == -1){
    Serial.println("Você é um administrador? (0)Não (1)Sim");
    a = 0;
  }
  while(k == 0){
    if(Serial.available() > 0){
      ad = Serial.readString();
      ad.trim();
      k = 12;                                                           //autoriza a atribuição final de novos usuários
      a = 1;                                                            //autoriza que a própria função retorne o status de administrador
    }
    if(a == 1){
      return ad;
    }
  }
}

String leituraNome(){                                                  //FUNÇÃO PARA A LEITURA TEMPORÁRIA DO NOME DE UM USUÁRIO
  if(n == -1){                                                         //tal função se difere da cadastroNome(); por não incrementar posições no vetor permanente
    Serial.print("Digite seu nome: ");
    n = 0;    
  }  
  while(k == 0){    
  while(Serial.available() > 1){    
   nm = Serial.readString();
   nm.trim(); 
   Serial.println(nm);  
    s = -1;    
    n = 1;
  }  
  if(n == 1){
  return nm;
  }
  }  
}

String leituraSenha(){                                                //FUNÇÃO PARA A LEITURA DE SENHA
  if(s == -1){                                                        //tal função se difere da cadastroSenha(); por não exibir a senha digitada e por não autorizar a execução de outras
    Serial.println("Digite sua senha: ");
    s = 0;   
  }
  while(k == 0){  
  while(Serial.available() > 1){    
   sn = Serial.readString();
   sn.trim();
   s = 1;
  }
  if(s == 1){
    return sn;
  }
  }  
}
