#include <iostream> 
#include <sstream> 
#include <vector> 
#include <set> 
#include <windows.h>

#define PLY_DEPTH 5 

typedef enum{
    CZARNY, BIALY, 
    CZARNA_DAMKA, BIALA_DAMKA, 
    PUSTY
} Pole; 


/**********************************************/
/*****  Klasa reprezentująca szachownicę  *****/
/**********************************************/
class Szachownica{
    public:
        int ruch; //reprezentuje kierunek, w ktorym elementy moga sie poruszac

        int zmien_ruch(){
            ruch = -ruch;
            return ruch;
        }
 
        int licznik_pionkow(Pole kolor ) const{
            int licznik(0);
            for(int i = 0; i < 64; ++i){
                if( pola[i] == kolor)
                ++licznik;
            }
            return licznik;
        }

        bool czy_pusty(int miejsce) const{
            if( !zgodny(miejsce)) return false;
            return pola[miejsce] == PUSTY;
        }

        bool czy_damka(int miejsce) const{
            return pola[miejsce] == CZARNA_DAMKA || pola[miejsce] == BIALA_DAMKA;
        }

        bool zgodny(int miejsce) const{
            return miejsce >= 0 && miejsce < 64;
        }

        int kolumna(int miejsce) const{
            return (miejsce%8)+1;
        }

        bool zgodny_ruch(int miejsce1, int miejsce2) const{
            if (!zgodny(miejsce1) || !zgodny(miejsce2)) return false;
            int k1 = kolumna(miejsce1);
            int k2 = kolumna(miejsce2);
            return ((k1>k2)?k1-k2:k2-k1)<3;
        }
        
        Pole kolor(int miejsce) const{
            if(pola[miejsce]==CZARNY || pola[miejsce]==CZARNA_DAMKA)
                return CZARNY;
            else if(pola[miejsce]==BIALY || pola[miejsce]==BIALA_DAMKA)
                return BIALY;
            else
                return PUSTY;
        }

        bool przeciwne_kolory(int miejsce1, int miejsce2) const{
            if (!zgodny(miejsce1) || !zgodny(miejsce2)) return false;
            if (czy_pusty(miejsce1) || czy_pusty(miejsce2)) return false;
            return kolor(miejsce1) != kolor(miejsce2);
        }

        Pole kolorp(Pole pionek) const{
            if(pionek==BIALY || pionek==BIALA_DAMKA) return BIALY;
            if(pionek==CZARNY || pionek==CZARNA_DAMKA) return CZARNY;
            return PUSTY;
        }

        bool przeciwne_koloryp(Pole pionek, int miejsce) const{
            if (!zgodny(miejsce) || czy_pusty(miejsce)) return false;
            return kolorp(pionek) != kolor(miejsce);
        }

        bool czy_zablokowany(int miejsce) const{
            if(czy_zablokowany(miejsce)) return true;
            if (czy_damka(miejsce)){
                int ms1 = miejsce - 7;
                int ms2 = miejsce - 9;
                int ms3 = miejsce + 9;
                int ms4 = miejsce + 7;
            if( (czy_pusty(ms1))
             || (czy_pusty(ms2))
             || (czy_pusty(ms3))
             || (czy_pusty(ms4)) ){
                 return false;
             }
             int j1 = miejsce - 14;
             int j2 = miejsce - 18;
             int j3 = miejsce + 18;
             int j4 = miejsce + 14;

             if( (przeciwne_kolory(miejsce,ms1) && czy_pusty(j1))
              || (przeciwne_kolory(miejsce,ms2) && czy_pusty(j2))
              || (przeciwne_kolory(miejsce,ms3) && czy_pusty(j3))
              || (przeciwne_kolory(miejsce,ms4) && czy_pusty(j4))){
                  return false;
              }
            }
            else{
                int ms1 = ruch * 7;
                int ms2 = ruch * 9;
                int j1 = ruch * 14;
                int j2 = ruch * 18;
                if(czy_pusty(ms1) || czy_pusty(ms2)) 
                    return false;
                if( (przeciwne_kolory(miejsce,ms1) && czy_pusty(j1))
                 || (przeciwne_kolory(miejsce,ms2) && czy_pusty(j2)) )
                    return false;
            }
            return true;
        }

        

        std::vector< std::vector<int> > policz_kolejne_bicia(
            std::vector<std::vector<int> > ruchy_jak_dotad,
            int                            miejsce,
            std::set<int>                  zbite,
            Pole                           pionek) const{
                if(pionek==CZARNA_DAMKA || pionek==BIALA_DAMKA
                || (pionek==CZARNY && miejsce==7)
                || (pionek==BIALY && miejsce ==0)){
                    pionek = (pionek==BIALY)?BIALA_DAMKA:pionek;
                    pionek = (pionek==CZARNY)?CZARNA_DAMKA:pionek;

                    std::vector<std::vector<int>> ruchy1=ruchy_jak_dotad;
                    std::vector<std::vector<int>> ruchy2=ruchy_jak_dotad;
                    std::vector<std::vector<int>> ruchy3=ruchy_jak_dotad;
                    std::vector<std::vector<int>> ruchy4=ruchy_jak_dotad;

                    int ms1 = miejsce - 7; int j1 = miejsce - 14;
                    int ms2 = miejsce - 9; int j2 = miejsce - 18;
                    int ms3 = miejsce + 9; int j3 = miejsce + 18;
                    int ms4 = miejsce + 7; int j4 = miejsce + 14;

                    bool bylo_bicie = false;
                    bool inc1 = false;
                    bool inc2 = false;
                    bool inc3 = false;
                    bool inc4 = false;

                    if(zbite.count(ms1)==0
                        && przeciwne_koloryp(pionek,ms1)
                        && czy_pusty(j1)
                        && zgodny_ruch(miejsce,j1)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy1[i].push_back(j1);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms1);
                            ruchy1 = policz_kolejne_bicia(ruchy1, j1, biciek, pionek);
                            inc1 = true;
                        }
                    if(zbite.count(ms2)==0
                        && przeciwne_koloryp(pionek,ms2)
                        && czy_pusty(j2)
                        && zgodny_ruch(miejsce,j2)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy2[i].push_back(j2);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms2);
                            ruchy2 = policz_kolejne_bicia(ruchy2, j2, biciek, pionek);
                            inc2 = true;
                        }
                    if(zbite.count(ms3)==0
                        && przeciwne_koloryp(pionek,ms3)
                        && czy_pusty(j3)
                        && zgodny_ruch(miejsce,j3)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy3[i].push_back(j3);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms3);
                            ruchy3 = policz_kolejne_bicia(ruchy3, j3, biciek, pionek);
                            inc3 = true;
                        }
                    if(zbite.count(ms4)==0
                        && przeciwne_koloryp(pionek,ms4)
                        && czy_pusty(j4)
                        && zgodny_ruch(miejsce,j4)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy4[i].push_back(j4);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms4);
                            ruchy4 = policz_kolejne_bicia(ruchy4, j4, biciek, pionek);
                            inc4 = true;
                        }
                    
                    if(bylo_bicie){
                        if(!inc1) ruchy1.clear();
                        if(inc2){
                            for(unsigned int i=0; i<ruchy2.size(); ++i){
                                ruchy1.push_back(ruchy2[i]);
                            }
                        }
                        if(inc3){
                            for(unsigned int i=0; i<ruchy3.size(); ++i){
                                ruchy1.push_back(ruchy4[i]);
                            }
                        }
                        if( inc4 ) 
                         { 
                    for(unsigned int i=0; i<ruchy4.size(); ++i) 
                                { 
                        ruchy1.push_back( ruchy4[i] ); 
                                } 
                        } 
                        return ruchy1;
                    }
                    else{
                        return ruchy_jak_dotad;
                    }
                }
                else{
                    std::vector<std::vector<int>>ruchy1 = ruchy_jak_dotad;
                    std::vector<std::vector<int>>ruchy2 = ruchy_jak_dotad;
                    int ms1 = miejsce+7*ruch;
                    int j1 = miejsce+14*ruch;
                    int ms2 = miejsce+9*ruch;
                    int j2 = miejsce+18*ruch;
                    bool bylo_bicie = false;
                    bool inc1 = false;
                    bool inc2 = false;

                      if(zbite.count(ms1)==0
                        && przeciwne_koloryp(pionek,ms1)
                        && czy_pusty(j1)
                        && zgodny_ruch(miejsce,j1)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy1[i].push_back(j1);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms1);
                            ruchy1 = policz_kolejne_bicia(ruchy1, j1, biciek, pionek);
                            inc1 = true;
                        }
                      if(zbite.count(ms2)==0
                        && przeciwne_koloryp(pionek,ms2)
                        && czy_pusty(j2)
                        && zgodny_ruch(miejsce,j2)){
                            bylo_bicie = true;
                            for(unsigned int i=0; i<ruchy_jak_dotad.size(); ++i){
                                ruchy2[i].push_back(j2);
                            }
                            std::set<int> biciek =zbite;
                            biciek.insert(ms2);
                            ruchy2 = policz_kolejne_bicia(ruchy2, j2, biciek, pionek);
                            inc2 = true;
                        }
                      if(bylo_bicie){
                          if(!inc1) ruchy1.clear();
                          if(inc2){
                              for(unsigned int i=0; i<ruchy2.size(); ++i){
                                  ruchy1.push_back(ruchy2[i]);
                              }
                          }
                          return ruchy1;
                      }
                      else{
                          return ruchy_jak_dotad;
                      }
                    
                    
                }
            }
 void wylicz_bicia_w_kolejnosci(  
        std::vector<std::vector<int> > &moves,  
        int miejsce,  
        std::set<int> zbite,  
        int origin ) const 
    { 
        std::vector<std::vector<int> > ruchy_jak_dotad; 
        std::vector<int> pierwsze_bicie; 
        pierwsze_bicie.push_back(miejsce);    
        ruchy_jak_dotad.push_back(pierwsze_bicie); 
         
        std::vector< std::vector<int> > nowy_ruch =  
            policz_kolejne_bicia( 
                ruchy_jak_dotad, 
                miejsce, 
                zbite, 
                pola[origin] 
            ); 
         
        // uporzadnkownie ich
        for(unsigned int i=0; i<nowy_ruch.size(); ++i) 
        { 
            moves.push_back( nowy_ruch[i] ); 
        } 
    } 


     std::vector<std::vector<int> > wylicz_mozliwe_ruchy(int miejsce, bool & bicia_tylko) const 
    { 
        std::vector<std::vector<int> > moves; 
        
        if( !zgodny(miejsce) || czy_pusty(miejsce) ) 
            return moves; 
        
        if( (kolor(miejsce)==CZARNY && ruch!=1 ) 
         || (kolor(miejsce)==BIALY && ruch!=-1 ) ) 
            return moves; 
        // skakanie jest obowiązkowe!!!, jeśli to możliwe, dlatego najpierw je policzmy
        if( czy_damka(miejsce) ) 
        { 
            int ms1 = miejsce - 7;    int j1 = miejsce - 14; 
            int ms2 = miejsce - 9;    int j2 = miejsce - 18; 
            int ms3 = miejsce + 9;    int j3 = miejsce + 18; 
            int ms4 = miejsce + 7;    int j4 = miejsce + 14; 
             
            if( przeciwne_kolory(miejsce,ms1) && czy_pusty(j1) && zgodny_ruch(miejsce,j1) ) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms1 ); 
                wylicz_bicia_w_kolejnosci( moves, j1, zbite, miejsce ); 
            } 
            if( przeciwne_kolory(miejsce,ms2) && czy_pusty(j2) && zgodny_ruch(miejsce,j2)) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms2 ); 
                wylicz_bicia_w_kolejnosci( moves, j2, zbite, miejsce ); 
            } 
            if( przeciwne_kolory(miejsce,ms3) && czy_pusty(j3) && zgodny_ruch(miejsce,j3)) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms3 ); 
                wylicz_bicia_w_kolejnosci( moves, j3, zbite, miejsce ); 
            } 
            if( przeciwne_kolory(miejsce,ms4) && czy_pusty(j4) && zgodny_ruch(miejsce,j4)) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms4 ); 
                wylicz_bicia_w_kolejnosci( moves, j4, zbite, miejsce ); 
            } 
            if( moves.size() > 0 || bicia_tylko ){ 
                bicia_tylko = true; 
                return moves;       
            } 
           
            if( czy_pusty(ms1) && zgodny_ruch(miejsce,ms1)) 
            { 
                std::vector<int> m; 
                m.push_back(ms1); 
                moves.push_back(m); 
            } 
            if( czy_pusty(ms2) && zgodny_ruch(miejsce,ms2)) 
            { 
                std::vector<int> m; 
                m.push_back(ms2); 
                moves.push_back(m); 
            } 
            if( czy_pusty(ms3) && zgodny_ruch(miejsce,ms3)) 
            { 
                std::vector<int> m; 
                m.push_back(ms3); 
                moves.push_back(m); 
            } 
            if( czy_pusty(ms4) && zgodny_ruch(miejsce,ms4)) 
            { 
                std::vector<int> m; 
                m.push_back(ms4); 
                moves.push_back(m); 
            } 
        } 
        else 
        { 
            int ms1 = miejsce + 7*ruch;    int j1 = miejsce + 14*ruch; 
            int ms2 = miejsce + 9*ruch;    int j2 = miejsce + 18*ruch; 
             
            if( przeciwne_kolory(miejsce,ms1) && czy_pusty(j1) && zgodny_ruch(miejsce,j1)) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms1 ); 
                wylicz_bicia_w_kolejnosci( moves, j1, zbite, miejsce ); 
            } 
            if( przeciwne_kolory(miejsce,ms2) && czy_pusty(j2) && zgodny_ruch(miejsce,j2)) 
            { 
                std::set<int> zbite; 
                zbite.insert( ms2 ); 
                wylicz_bicia_w_kolejnosci( moves, j2, zbite, miejsce ); 
            } 
             
            if( moves.size() > 0 || bicia_tylko ) 
            { 
                bicia_tylko = true; 
                return moves;  
            } 
                 
            if( czy_pusty(ms1) && zgodny_ruch(miejsce,ms1)) 
            { 
                std::vector<int> m; 
                m.push_back(ms1); 
                moves.push_back(m); 
            } 
            if( czy_pusty(ms2) && zgodny_ruch(miejsce,ms2)) 
            { 
                std::vector<int> m; 
                m.push_back(ms2); 
                moves.push_back(m); 
            } 
             
        } 
        return moves; 
    } 
     
      std::vector<int> policz_aktywne_pionki() 
    { 
        std::vector<int> pionki; 
         
        for(int i=0; i<64; ++i) 
        { 
            if( ruch==1 && (pola[i]==CZARNY || pola[i]==CZARNA_DAMKA) ) 
            { 
                pionki.push_back(i); 
            } 
            if( ruch==-1 && (pola[i]==BIALY || pola[i]==BIALA_DAMKA) ) 
            { 
                pionki.push_back(i); 
            } 
        } 
         
        return pionki; 
    } 
    
        Pole pola[64]; 

     void reset() 
    { 
        for(int i=0; i<64; ++i) 
            pola[i] = PUSTY; 
         
        for(int i=0; i<8; i+=2) 
        { 
            pola[i+1 ] = CZARNY; 
            pola[i+8 ] = CZARNY; 
            pola[i+17] = CZARNY; 
            pola[i+40] = BIALY; 
            pola[i+49] = BIALY; 
            pola[i+56] = BIALY; 
        } 
    } 
     
    Szachownica() 
    { 
        ruch = -1; 
        reset(); 
    } 

    Szachownica zrobRuch( std::vector<int> move ) 
    { 
        Szachownica c = *this; 
         
        
        int origin = move[0]; 
        Pole pionek = c.pola[origin]; 
        c.pola[origin] = PUSTY; 
        int obecny = origin; 
        for(unsigned int i=1; i<move.size(); ++i) 
        { 
            if( kolumna(obecny) - kolumna(move[i]) == 2  
             || kolumna(obecny) - kolumna(move[i]) == -2 ) 
            { 
                int delta = obecny - move[i]; 
                
                c.pola[ move[i] + delta/2 ] = PUSTY; 
            } 
            obecny = move[i]; 
        } 
        int landing = move[move.size()-1]; 
         
        c.pola[landing] = pionek; 
        if( landing/8==0 && pionek==BIALY ) 
        { 
            c.pola[landing] = BIALA_DAMKA; 
        } 
        if( landing/8==7 && pionek==CZARNY ) 
        { 
            c.pola[landing] = CZARNA_DAMKA; 
        } 
        c.zmien_ruch(); 
        return c; 
    } 
};



/***********************************************************************/
/*****   Klasa reprezentujaca drzewo mozliwosci algorytmu minimax  *****/
/***********************************************************************/
class Drzewo 
{ 
public: 
    Szachownica korzen; 
    std::vector<Drzewo> potomek; 
    std::vector<std::vector<int> > moves; 
    int ply; 
     

    Drzewo( Szachownica &basis, int ply ) 
    { 
        korzen = basis; 
        if( ply > 0 ) 
        { 
            do_ply(ply); 
        } 
        this->ply = ply; 
    } 
     
    bool czy_remis() 
    { 
        return (moves.size()==0)&&!czy_wygrany(); 
    } 
     
    void wybierz_ruch( int move_n ) 
    { 
        korzen = potomek[move_n].korzen; 
        moves = potomek[move_n].moves; 
        std::vector<Drzewo> chiltemp = potomek[move_n].potomek; 
        potomek = chiltemp; 
        
        do_ply(ply); 
    } 
     
    bool czy_wygrany() 
    { 
        return 
            (korzen.licznik_pionkow( CZARNY ) == 0 && korzen.licznik_pionkow(CZARNA_DAMKA)==0) 
          ||(korzen.licznik_pionkow( BIALY ) == 0 && korzen.licznik_pionkow(BIALA_DAMKA)==0); 
    } 
     
    void do_ply( int pozostala_glebokosc ) 
    { 
        if( pozostala_glebokosc > 0 && moves.size() == 0 ) 
        { 
            potomek.clear(); 
            std::vector<Szachownica> mozliwe_ruchy; 
             
            std::vector<int> pionki = korzen.policz_aktywne_pionki(); 
             
            bool bicia_tylko = false; 
             
            for(unsigned int i=0; i<pionki.size(); ++i) 
            { 
                bool bicia_tylko_wczesniejsze = bicia_tylko; 
                std::vector<std::vector<int> > pionki_ruch = korzen.wylicz_mozliwe_ruchy( pionki[i], bicia_tylko ); 
                if( bicia_tylko_wczesniejsze != bicia_tylko ) 
                { 
                    moves.clear(); 
                    potomek.clear(); 
                } 
                 
                for(unsigned int mv=0; mv<pionki_ruch.size(); ++mv) 
                { 
                    std::vector<int> move = pionki_ruch[mv]; 
                    move.insert( move.begin(), pionki[i] ); 
                     
                    Szachownica mb = korzen.zrobRuch( move ); 
                     
                    potomek.push_back( Drzewo( mb, pozostala_glebokosc-1 ) ); 
                    moves.push_back(move); 
                } 
            } 
        } 
        else 
        { 
            for(unsigned int i=0; i<potomek.size(); ++i) 
            { 
                potomek[i].do_ply( pozostala_glebokosc-1 ); 
            } 
        } 
    } 
}; 


int wiersz(int miejsce) { 
    return 7-miejsce/8; 
} 
 
char kol(int miejsce) { 
    return (char)( miejsce%8 + 'a' ); 
} 
 
std::string p_koord( int miejsce ) 
{ 
    std::string koord("  "); 
    koord[0] = kol(miejsce); 
    koord[1] = (char)(wiersz(miejsce)+'1'); 
    return koord; 
} 


void wyswietl_szachownice( Drzewo & drzewo ) 
{ 
    Szachownica &plansza = drzewo.korzen; 
     
    std::vector<std::vector<int> > &moves = drzewo.moves; 
     
    std::vector<std::string> move_tbl; 
     
    for(int i=0; i<17; ++i) 
    { 
        std::stringstream linia; 
        for(int j=i; j<moves.size(); j+=17) 
        { 
            linia << j << ") "; 
            for(int k=0; k<moves[j].size(); ++k) 
            { 
                linia << p_koord( moves[j][k] ); 
                if( k < moves[j].size() - 1 ) 
                linia << "-"; 
            } 
            linia << std::string("\t\t"); 
        } 
        move_tbl.push_back(linia.str()); 
    } 
    std::cout << "\n"; 
    std::cout << "     a   b   c   d   e   f   g   h\n"; 
    std::string hline = std::string(33,'-'); 
    for(int i=0; i<8; ++i) 
    { 
        std::cout << "   " << hline << "  " << move_tbl[2*i] << "\n"; 
        std::cout << " " << (8-i) << " "; 
        for(int j=0; j<8; ++j) 
        { 
            std::cout << "|"; 
            if( plansza.pola[i*8+j]==CZARNY ) 
                std::cout << " o "; 
            else if( plansza.pola[i*8+j]==BIALY ) 
                std::cout << " * "; 
            else if( plansza.pola[i*8+j]==CZARNA_DAMKA ) 
                std::cout << " O "; 
            else if( plansza.pola[i*8+j]==BIALA_DAMKA ) 
                std::cout << " # "; 
            else 
                std::cout << "   "; 
        } 
        std::cout << "|  " << move_tbl[2*i+1] << "\n"; 
    } 
    std::cout << "   " << hline << "  " << move_tbl[16] << "\n"; 
    std::cout << "     a   b   c   d   e   f   g   h\n"; 
} 
 
  
/***************************************************/
/*****  Klasa reprezentujaca algorytm minimax  *****/
/***************************************************/
 class Minimax 
{ 
public: 
    Minimax() {

    }
     
    int wynik_bazowy( Szachownica &plansza ) const 
    {    
        int wynik = 0; 
        if( plansza.ruch==1 ) // czarne
        { 
            wynik = plansza.licznik_pionkow(CZARNA_DAMKA)*2; 
            wynik += plansza.licznik_pionkow(CZARNY); 
            wynik -= plansza.licznik_pionkow(BIALA_DAMKA)*2; 
            wynik -= plansza.licznik_pionkow(BIALY); 
        } 
        else 
        { 
            wynik = plansza.licznik_pionkow(BIALA_DAMKA)*2; 
            wynik += plansza.licznik_pionkow(BIALY); 
            wynik -= plansza.licznik_pionkow(CZARNA_DAMKA)*2; 
            wynik -= plansza.licznik_pionkow(CZARNY); 
        } 
        return wynik; 
    } 
     
    int max_wynik( Drzewo &drzewo ) const 
    { 
        unsigned int len = drzewo.potomek.size(); 
        int maxwynik = -7000; 
        int wynik; 
        for(unsigned int i=0; i<len; ++i) 
        { 
            wynik = wynik_bazowy(drzewo.potomek[i].korzen);  
            // wydedujuj zagrozenia
            wynik -= max_wynik( drzewo.potomek[i] ); 
            if( wynik > maxwynik ) 
            { 
                maxwynik = wynik; 
            } 
        } 
        if( len==0 ) 
            return 0; 
        return maxwynik; 
    } 
     
    int wybierz_ruch( Drzewo &drzewo ) const { 
        unsigned int len = drzewo.potomek.size(); 
        int wyniki[len]; 
        int najlepszy_wynik = -7000; 
        int najlepszy_index = 0; 
        
        for(unsigned int i=0; i<len; ++i) 
        { 
            wyniki[i] = max_wynik( drzewo.potomek[i] ); 
            if( wyniki[i] > najlepszy_wynik ) 
            { 
                najlepszy_wynik = wyniki[i]; 
                najlepszy_index = i; 
            } 
        } 
        return najlepszy_index; 
    } 
}; 
 

/**********************************************************************/
/*************             MAIN PROGRAMU                 **************/
/**********************************************************************/

 int main( int argc, char **argv ) 
{   
    std::cout<<"\n\n\n\n\n___________________________________________________________________________________  \n\n";Sleep(100);
    std::cout<<".##......##....###....########...######.....###....########..##....##\n";Sleep(100);
    std::cout<<".##..##..##...##.##...##.....##.##....##...##.##...##.....##..##..##.\n";Sleep(100);
    std::cout<<".##..##..##..##...##..##.....##.##........##...##..##.....##...####..\n";Sleep(100);
    std::cout<<".##..##..##.##.....##.########..##.......##.....##.########.....##...\n";Sleep(100);
    std::cout<<".##..##..##.#########.##...##...##.......#########.##.....##....##...\n";Sleep(100);
    std::cout<<".##..##..##.##.....##.##....##..##....##.##.....##.##.....##....##...\n";Sleep(100);
    std::cout<<"..###..###..##.....##.##.....##..######..##.....##.########.....##...  \n";Sleep(100);
    std::cout<<"___________________________________________________________________________________  \n\n";Sleep(100);
    std::cout<<"                        Wykonane przez Jana Binkowskiego  \n\n\n\n\n";Sleep(500);


    Szachownica plansza; 
    Drzewo drzewo( plansza, PLY_DEPTH ); 
    Minimax minimax; 
     
    bool rezygnuj = false; 
     
    int ruch_komputera = -1; 
    while(!(rezygnuj || drzewo.czy_wygrany() || drzewo.czy_remis() )) 
    { 
        int move; 

        wyswietl_szachownice( drzewo ); 
        std::cout << "\n\n\n    Twoj ruch (-1 = wyjdz, -2 = rusz sie za mnie):   "; 
        std::cin >> move; 
        if( !std::cin.fail() || (move > -3 && move <drzewo.potomek.size())) 
        { 
            if( move >= 0 ) 
            { 
                drzewo.wybierz_ruch(move); 
                wyswietl_szachownice(drzewo); 
            } 
            else if( move==-2) 
            { 
                ruch_komputera = minimax.wybierz_ruch(drzewo); 
                std::cout << "\n    Wybralem " << ruch_komputera << " za Ciebie.\n"; 
                drzewo.wybierz_ruch(ruch_komputera); 
            } 
            else 
            { 
                rezygnuj = true; 
            } 
            if( !drzewo.czy_wygrany() && !drzewo.czy_remis() && !rezygnuj ) 
            { 
                wyswietl_szachownice(drzewo); 
                ruch_komputera = minimax.wybierz_ruch(drzewo); 
                std::cout << "\n    Komputer: Wybralem " << ruch_komputera << ".\n"; 
                drzewo.wybierz_ruch(ruch_komputera); 
            } 
        } 
        else 
        { 
            std::cin.clear(); 
            std::cin.ignore(); 
            std::cout << "\n\n    Nieprawidlowy ruch pionkiem.\n"; 
        } 
    } 
     
    // wyswietl kto wygral
    if(!rezygnuj) 
    { 
        if( drzewo.czy_remis() ) 
        { 
            std::cout << "    Mecz zakonczony remisem.\n"; 
        } 
        else if( drzewo.korzen.licznik_pionkow(CZARNY) + drzewo.korzen.licznik_pionkow(CZARNA_DAMKA) > 0 ) 
        { 
            std::cout << "    Wyglada na to ze wygralem.\n"; 
            std::cout << "    Wspaniala gra. Dzieki za udzial.\n"; 
        } 
        else 
        { 
            std::cout << "    Wygrales te gre!\n"; 
        } 
    } 
    else 
    { 
        std::cout<< "    Do zobaczenia.\n"; 
    } 
    return 0; 
} 
 

 
 
 