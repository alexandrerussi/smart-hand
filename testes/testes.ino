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

int botaoBraco = 38;

int ledPin = 42;

void setup() {
  pinMode(botao, INPUT);
  pinMode(botaoBraco, INPUT);
  pinMode(ledPin, OUTPUT)
  
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
  {
    botaoDesativado = true;
    digitalWrite(ledPin, HIGH);
  }
  else if(qualidade == 100)
  {
    digitalWrite(ledPin, LOW);
  }
    
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
}

void alterarPosicaoServo(int intervaloReal)
{
  if (!motorLiberado || botaoDesativado)
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
