#include "Player.h"
#include "Tabella.h"
#include "Nave.h"
#include "Coordinate.h"
#include <iostream>


Player::Player()
{
  _Plancia.createFlotta();
  _Plancia.createRadar();
}


std::string Player::getName()
{
  return _nome;
}


void Player::setName(std::string Nome)
{
  _nome = Nome;
}


bool Player::Check(Coordinate c1, Coordinate c2) //controlla se casella di partenza e di arrivo sono compatibili
{
  int x1 = c1.getX();
  int y1 = c1.getY();
  int x2 = c2.getX();
  int y2 = c2.getY();

  if (x2 < 0 || x2 > _Plancia.getN()-1)
    return false;
  if (y2 < 0 || y2 > _Plancia.getN()-1)
    return false;
  if (x1==x2)
  {
    int max = (y2 > y1)? y2 : y1;
    int min = (y2 < y1)? y2 : y1;
    for (int i = min; i <=  max; i++)
    {
      if (_Plancia[i][x1] != Flotta::Sea)
        return false;
    }
    return true;
  } else if (y1==y2)
  {
    int max = (x2 > x1)? x2 : x1;
    int min = (x2 < x1)? x2 : x1;
    for (int i = min; i <= max; i++)
    {
      if (_Plancia[y1][i] != Flotta::Sea)
        return false;
    }
    return true;
  } else
  {
    std::cout << "ERRORE IN CHECK(Player.cpp) \n";
    return false;
  }
}


Nave Player::setShips(int len, Coordinate coord){ //crea e pone le navi
    int l = len - 1;
    Coordinate U,D,R,L;
    U = coord + Coordinate(0,-l);
    D = coord + Coordinate(0,+l);
    R = coord + Coordinate(+l,0);
    L = coord + Coordinate(-l,0);

    bool u = Check(coord, U);
    bool d = Check(coord, D);
    bool r = Check(coord, R);
    bool le = Check(coord, L);

    //bool le = Check(x1,y1,x1-l,y1);
    //bool u = Check(x1,y1,x1,y1-l);
    //bool d = Check(x1,y1,x1,y1+l);

    if(!u && !d && !r && !le)
    {
      std::cout << "not valid\n Prova coordinate valide\n";

      Coordinate A;
      A.getFromPlayer(_Plancia.getN());
      return setShips(len,A);
    }
    if (u)
    {
      std::cout << "Premi u per mettere la nave in ";
      U.print();
      std::cout << "\n";
    }
    if (d)
    {
      std::cout << "Premi d per mettere la nave in ";
      D.print();
      std::cout << "\n";
    }
    if (le)
    {
      std::cout << "Premi l per mettere la nave in ";
      L.print();
      std::cout << "\n";
    }
    if (r)
    {
      std::cout << "Premi r per mettere la nave in ";
      R.print();
      std::cout << "\n";
    }
    char direzione;
    std::cin >> direzione;
    std::cin.ignore(10000,'\n');
    if (direzione == 'u' && u)
    {
      _Plancia.setNave(coord, U);
      return Nave(coord, U);
    }
    if (direzione == 'd' && d)
    {
      _Plancia.setNave(coord, D);
      return Nave(coord, D);
    }
    if (direzione == 'l' && le)
    {
      _Plancia.setNave(coord, L);
      return Nave(coord, L);
    }
    if (direzione == 'r' && r)
    {
      _Plancia.setNave(coord, R);
      return Nave(coord, R);
    }
    std::cout << "not valid \n Prova direzione valida\n";
    return setShips(len,coord);
}


void Player::Mozzo(int i, int lunghezza) //chiede le coordinate delle navi da creare
{
  Coordinate A;
  if(A.getFromPlayer(_Plancia.getN()))
  {
    _navi[i] = setShips(lunghezza, A);
    std::cout << std::string(100,'\n'); //"aggiorna" schermo
    PrintFlo();
    std::cout << std::string(12,'\n');

  }else
  {
    std::cout << "Qualcosa è andato storto in Mozzo!" << '\n';
  }

}


void Player::PrintRad() //stampa lo schermo di un giocatore senza la flotta
{
  std::cout << "\n\t\t\tCampo nemico\n\n";
  _Plancia.PrintRadar();
}

void Player::PrintFlo() //stampa lo schermo di un giocatore con la flotta
{
  std::cout << "\n\t\t\tLa tua flotta\n\n";
  _Plancia.PrintFlotta();
}


void Player::Attack(Player * Other) //dichiara un attacco
{
  Coordinate A;

  if(!A.getFromPlayer(_Plancia.getN()))
  {
    std::cout << "Coordinate fuori range \n";
    Attack(Other);
  }

  int x = A.getX();
  int y = A.getY();
  if (!Other->_Plancia.getRadar(x,y))
  {
    std::cout << "Quadrante già colpito" << '\n';
    Attack(Other);
  }else
  {
    std::cout << std::string(100,'\n'); //"aggiorna" schermo
    if(Other->_Plancia.setRadar(x,y)) //Possibilità di fare overload di setradar per non prendere necessariamente flotta
      {
        std::cout << "Colpito!\n";
        colpi_a_segno++;
      }
      else
      {
        std::cout << "Mancato!\n";
      }
    if(Other->Sunk(x,y))
    {
      navi_affondate++;
    }
    //Spostiamo Other._Plancia.setRadar in Hit()?
    colpi_sparati++;
  }
}

//Eliminiamo Player::Hit e mettiamo direttamente il for in Attack?
bool Player::Sunk(int x, int y) // Dichiara se l'attacco ha Affondato una nave
{
  Coordinate A(x,y);
  for (int i = 0; i < _n; i++)
  {
    if(_navi[i].Hit(A))
    {
      std::cout << "Affondata nave di "<<_nome << "!\n";

      for (int j = 0; j < _navi[i].getlunghezza(); j++)
      {
        for(int h = (_navi[i])[j].getY()-1; h <= (_navi[i])[j].getY()+1; h++)
        {
          for(int k = (_navi[i])[j].getX()-1; k <= (_navi[i])[j].getX()+1; k++)
          {
            if((h > -1) && (h < _Plancia.getN()) && (k > -1) && (k < _Plancia.getN()) && (_Plancia.getRadar(k,h)))
            {
              _Plancia.setRadar(k,h);
            }
          }
        }
      }
      _contatore--;
      return true;
    }
  }
  return false;
}


int Player::getContatore() const //restituisce il numero delle navi sopravvissute
{
  return _contatore;
}

void Player::Riempimento() //Riempie la plancia chiamando mozzo nNavi volte
{
  std::cout << std::string(100,'\n');
  std::cout << _nome << ", inizia la fase di creazione..."<< '\n';
  std::cout << "Inserisci la tua portaerei" << '\n';
  Mozzo(0,5);
  std::cout << "Inserisci la tua corazzata" << '\n';
  Mozzo(1,4);
  std::cout << "Inserisci i tuoi incrociatori" << '\n';
  Mozzo(2,3);
  Mozzo(3,3);
  std::cout << "Inserisci il tuo cacciatorpediniere" << '\n';
  Mozzo(4,2);
  std::string a;
  std::cout << "Premi un tasto qualsiasi" << '\n';
  std::cin >> a;
  _Plancia.Greta();
  std::cout << std::string(100,'\n');
}

void Player::Stats()
{
  std::cout << _nome << ":" << '\n';
  std::cout << "\tNumero di colpi sparati:\t\t" << colpi_sparati << '\n';
  std::cout << "\tNumero di colpi a segno:\t\t " << colpi_a_segno << '\n';
  std::cout << "\tPrecisione:\t\t\t\t" << 100*(float)colpi_a_segno/(float)colpi_sparati << "%\n\n";
  std::cout << "\tNumero di navi nemiche affondate:\t" << navi_affondate << '\n';
  std::cout << "\tNumero di navi superstiti:\t\t" << _contatore << '\n';

}
