#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "rng.h"


int main(int argc, char *argv[]) {

    int i,zhoda=0;
    for (i=0;i<MONSTER_TYPE_COUNT;i++){                         //zistenie, ci je spravne zadany monster, ak nie vrati 1
        if (strcmp(monster_types[i].name,argv[1])==0) { zhoda++; break; }
    }
    if (zhoda==0) { return 1;}

    long int pocet_jednotiek,seed;                              // premena stringov z argv na cisla
    char* pEnd;
    pocet_jednotiek=strtol(argv[2],&pEnd,0);
    seed=strtol(argv[3],&pEnd,0);
    srnd(seed);

    int level=0;
    Unit monster= {monster_types+i,MONSTER_INITIAL_HP,level};           //inicializacia monstra
    Unit armada[pocet_jednotiek+1];

     if (argc==6){
        FILE* fp;
        fp=fopen(argv[5],"r");
        if (fp==NULL) { return 2; }
        else {
            for (i=0;i<ENEMY_TYPE_COUNT;i++){
                if (fscanf(fp,"%s %d %d",enemy_types[i].name,&enemy_types[i].att,&enemy_types[i].def)!=3) { return 3; }
            }
        }
    fclose(fp);
    }

    for (i = 0; i < pocet_jednotiek; i++) {                                         //inicializacia armady
        armada[i].type = enemy_types + rnd(0, ENEMY_TYPE_COUNT - 1);
        armada[i].hp = rnd(ENEMY_MIN_INIT_HP, ENEMY_MAX_INIT_HP);
        armada[i].level = rnd(0, UNIT_MAX_LEVEL);}

     printf("%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n",monster.type->name,monster.type->att,monster.type->def,monster.hp,monster.level);
    for (i=0;i<pocet_jednotiek;i++){
        printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n",i,armada[i].type->name,armada[i].type->att,armada[i].type->def,armada[i].hp,armada[i].level);
    }       printf("\n");

    int min, indmin, zaciatok = 0, dmg = 0;
    int base_dmg=0, strength=0,defense=0,monster_dmg = 0, enemy_dmg = 0,nazive=0;

while(1) {
    if (monster.hp<=0) { break; }                       //kontrola, ci je monstrum este nazive
    nazive=0;
    for (i=0;i<=pocet_jednotiek-1;i++){                 //kontrola, ci je niekto z armady nazive
        if (armada[i].hp>0) { nazive++; }
    }
    if  (nazive==0) { break; }

    while (armada[zaciatok].hp <= 0) {
        zaciatok++;
    }
    min = armada[zaciatok].hp;
    indmin = zaciatok;
    for (i = zaciatok; i <= pocet_jednotiek - 1; i++) {
        if (min > armada[i].hp && armada[i].hp > 0) {
            min = armada[i].hp;
            indmin = i;
        }
    }
                                                        //utok monstra na nepriatela s najmensim hp
    base_dmg=30+monster.level-armada[indmin].level;
    strength=100+rnd(1,monster.hp)+monster.type->att;
    defense=100+rnd(1,armada[indmin].hp)+armada[indmin].type->def;
    dmg=(base_dmg*strength)/defense;
    monster_dmg=monster_dmg+dmg;

    armada[indmin].hp=armada[indmin].hp-dmg;
    printf("%s => %d => [%d] %s\n",monster.type->name,dmg,indmin,armada[indmin].type->name);

    for (i=0;i<=pocet_jednotiek-1;i++){
        if (armada[i].hp>0 && monster.hp>0) {
            base_dmg = 30 + armada[i].level - monster.level;
            strength = 100 + rnd(1, armada[i].hp) + armada[i].type->att;
            defense = 100 + rnd(    1, monster.hp) + monster.type->def;
            dmg = (base_dmg * strength) / defense;
            enemy_dmg = enemy_dmg + dmg;
            monster.hp=monster.hp-dmg;
            printf("[%d] %s => %d => %s\n",i,armada[i].type->name,dmg,monster.type->name);
        }

    }
    if (monster.hp>0 && monster.level<10) { monster.level++; }
    printf("\n");
    printf("%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n",monster.type->name,monster.type->att,monster.type->def,monster.hp,monster.level);
    for (i=0;i<pocet_jednotiek;i++){
        printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n",i,armada[i].type->name,armada[i].type->att,armada[i].type->def,armada[i].hp,armada[i].level);
    }       printf("\n");
}

    if (monster.hp<=0) { printf("Winner: Enemy\n"); }
    else {printf("Winner: %s\n",monster.type->name); }
    printf("Total monster DMG: %d\n",monster_dmg);
    printf("Total enemies DMG: %d\n",enemy_dmg);

    return 0;
}
