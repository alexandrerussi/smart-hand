#include <Mindwave.h>
#include <SimpleTimer.h>
#include <Servo.h>

#define SERVO 6 // Porta Digital 6 PWM

Mindwave mindwave;

SimpleTimer simpleTimer;

Servo s;

int concentracaoNecessaria = 30;
int duracaoDelay = 1000;

int atencao = 0;
int posicaoAtual = 0;
int intervaloPosicao = 180;

bool motorLiberado = true;

int botao = 40;
bool botaoDesativado = false;
int botao1 = 41;
bool botaoDesativado1 = false;
int botao2 = 42;
bool botaoDesativado2 = false;
int botao3 = 43;
bool botaoDesativado3 = false;
int botao4 = 44;
bool botaoDesativado4 = false;
int botao5 = 45;
bool botaoDesativado5 = false;
int botao6 = 46;
bool botaoDesativado6 = false;

int botaoBraco = 38;

void setup() {
  pinMode(botao, INPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);
  pinMode(botao5, INPUT);
  pinMode(botao6, INPUT);
  pinMode(botaoBraco, INPUT);
  
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(0); // Inicia motor posição zero
  Serial.begin(MINDWAVE_BAUDRATE);
}

void onMindwaveData()
{
  Serial.print("\tquality: ");
  Serial.print(mindwave.quality());
  Serial.print("\tattention: ");
  Serial.print(mindwave.attention());
  Serial.print("\tmeditation: ");
  Serial.print(mindwave.meditation());
  Serial.print("\tlast update: ");
  Serial.print(mindwave.lastUpdate());
  Serial.print("ms ago");
  Serial.println();
  
  atualizarMotor();
}

void liberarMovimentacaoMotor()
{
  Serial.println("liberarMovimentacaoMotor");
  motorLiberado = true;
}

void atualizarMotor()
{
  int qualidade = mindwave.quality();

  if (qualidade != 100)
    botaoDesativado = true;
    
  atencao = mindwave.attention();
  
  if (atencao > concentracaoNecessaria)
  {
    // Avança uma posição

    alterarPosicaoServo(intervaloPosicao);
  }
  else
  {
    // Volta uma posição

    alterarPosicaoServo(intervaloPosicao * -1);
  }
}

void pressionarBotao()
{
  bool leituraBotao = digitalRead(botao);
  botaoDesativado = leituraBotao;

  bool leituraBotao1 = digitalRead(botao1);
  botaoDesativado1 = leituraBotao1;

  bool leituraBotao2 = digitalRead(botao2);
  botaoDesativado2 = leituraBotao2;

  bool leituraBotao3 = digitalRead(botao3);
  botaoDesativado3 = leituraBotao3;

  bool leituraBotao4 = digitalRead(botao4);
  botaoDesativado4 = leituraBotao4;

  bool leituraBotao5 = digitalRead(botao5);
  botaoDesativado5 = leituraBotao5;

  bool leituraBotao6 = digitalRead(botao6);
  botaoDesativado6 = leituraBotao6;
}

void alterarPosicaoServo(int intervaloReal)
{
  if (!motorLiberado || botaoDesativado || botaoDesativado1 || botaoDesativado2 || botaoDesativado3 || botaoDesativado4 || botaoDesativado5 || botaoDesativado6)
    return;
  
  Serial.println("alterarPosicaoServo");
  
  posicaoAtual = constrain(posicaoAtual + intervaloReal, 0, 180);

  s.write(posicaoAtual);

  motorLiberado = false;
  
  simpleTimer.setTimeout(duracaoDelay, liberarMovimentacaoMotor);
}

void pressionarBotaoBraco()
{
  bool leituraBotaoBraco = digitalRead(botaoBraco);
  
  if (leituraBotaoBraco)
  {
  	for (posicaoAtual = posicaoAtual; posicaoAtual >= 0; posicaoAtual--)
  	{
  		s.write(posicaoAtual);
  		delay(50);
  	}
  }
}

void loop()
{
  simpleTimer.run();

  mindwave.update(Serial, onMindwaveData);
  
  pressionarBotao();

  pressionarBotaoBraco();

  //atualizarMotor();
}
