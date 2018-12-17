/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -L C++ -s 1 /tmp/bolsaKeys  */
/* Computed positions: -k'1-6' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif


#define TOTAL_KEYWORDS 697
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 7
#define MIN_HASH_VALUE 56
#define MAX_HASH_VALUE 4955
/* maximum key range = 4900, duplicates = 0 */

class Perfect_Hash
{
public:
  static inline unsigned int hash (const char *str, size_t len);
  static const char *in_word_set (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  static unsigned short asso_values[] =
    {
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,   20,  775,
        15,    5,    0,  593,  279,   20,   45,   20, 4956,    5,
      4956, 4956, 4956, 4956, 4956,  936,  220,  680,   10,   16,
        72,   70,  775,  335,   60,   10,  330,  175,   30,   61,
       595,  638,  320,   20,   95,    0,  670, 1016,  255,  730,
       782,  326, 1008,  486,  685,  136,  110,  535,  875,   20,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956, 4956,
      4956, 4956, 4956, 4956, 4956
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[5])];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[static_cast<unsigned char>(str[4])];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[static_cast<unsigned char>(str[3]+9)];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[static_cast<unsigned char>(str[2])];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[static_cast<unsigned char>(str[1]+6)];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0]+1)];
        break;
    }
  return hval;
}

const char *
Perfect_Hash::in_word_set (const char *str, size_t len)
{
  static const char * wordlist[] =
    {
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "CEEB3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "REDE3",
      "", "", "", "",
      "TEKA4",
      "", "", "", "",
      "TEKA3",
      "CHKE34",
      "", "", "", "", "",
      "CIEL3",
      "", "",
      "TESA3",
      "", "",
      "EEEL4",
      "", "", "", "",
      "EEEL3",
      "",
      "RADL3",
      "", "", "", "",
      "CMSA4",
      "", "",
      "DISB34",
      "",
      "CMSA3",
      "TAEE4",
      "", "", "", "",
      "TAEE3",
      "", "", "",
      "SHUL4",
      "JHSF3",
      "", "", "",
      "SHUL3",
      "", "", "", "", "", "", "", "", "",
      "MNDL3",
      "", "", "", "", "", "", "", "", "",
      "",
      "METB34",
      "", "", "", "", "", "", "", "",
      "FESA4",
      "", "", "", "",
      "FESA3",
      "", "",
      "NIKE34",
      "", "",
      "DASA3",
      "", "", "", "", "",
      "DAGB33",
      "", "", "",
      "JNJB34",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "TIFF34",
      "", "", "", "",
      "SANB4",
      "", "", "", "",
      "SANB3",
      "CEED4",
      "", "", "",
      "SNSL3",
      "CEED3",
      "", "", "",
      "TEND3",
      "MGEL4",
      "", "", "", "",
      "MGEL3",
      "", "", "", "",
      "FIGE4",
      "", "", "",
      "CATA4",
      "FIGE3",
      "", "", "",
      "CATA3",
      "RNEW4",
      "", "", "", "",
      "RNEW3",
      "", "", "",
      "MEND3",
      "", "", "", "", "", "", "", "", "",
      "CAML3",
      "", "", "", "",
      "CAMB4",
      "", "", "", "",
      "CAMB3",
      "", "", "", "", "", "", "", "", "",
      "",
      "AHEB3",
      "", "", "",
      "CEDO4",
      "TGTB34",
      "", "", "",
      "CEDO3",
      "", "", "", "", "", "", "", "", "",
      "",
      "SHOW3",
      "", "", "",
      "SOND3",
      "LMTB34",
      "", "", "",
      "CRDE3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "ENEV3",
      "CEEB6",
      "", "", "", "",
      "AZUL4",
      "TEXA34",
      "", "", "", "", "", "", "", "", "",
      "AETB34",
      "", "", "", "", "", "", "", "",
      "TELB4",
      "",
      "CRFB3",
      "", "",
      "TELB3",
      "", "", "", "", "",
      "FMXB34",
      "", "", "", "", "",
      "AIGB34",
      "DUKB34",
      "",
      "WEGE3",
      "", "", "", "",
      "RCSL4",
      "", "", "", "",
      "RCSL3",
      "HONB34",
      "", "", "",
      "TAXA4",
      "MOSC34",
      "", "", "",
      "TAXA3",
      "", "", "",
      "DHER34",
      "IGTA3",
      "", "", "", "",
      "TAXA2",
      "FDXB34",
      "", "", "",
      "TAXA0",
      "", "", "", "", "", "", "", "", "",
      "TGMA3",
      "", "", "", "",
      "TCSA3",
      "CEGR3",
      "", "", "", "", "", "", "", "",
      "RAIL3",
      "", "", "", "",
      "RDNI3",
      "", "", "", "",
      "RANI4",
      "", "", "", "",
      "RANI3",
      "MDLZ34",
      "FHER3",
      "", "",
      "LAME4",
      "",
      "SEER3",
      "", "",
      "LAME3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "SMTO3",
      "DMMO3",
      "", "", "", "", "", "", "", "", "",
      "",
      "AZEV4",
      "", "",
      "FRTA3",
      "",
      "AZEV3",
      "", "",
      "MDIA3",
      "", "", "", "", "", "", "", "", "",
      "HETA4",
      "CYRE3",
      "",
      "ELEK4",
      "",
      "HETA3",
      "NEMO3",
      "",
      "ELEK3",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "ROMI3",
      "MDTC34",
      "", "",
      "MEND6",
      "", "", "", "", "", "",
      "SANC34",
      "ROST34",
      "", "",
      "HAGA4",
      "", "", "", "",
      "HAGA3",
      "", "", "", "", "",
      "HOME34",
      "LHER4",
      "", "", "",
      "RENT3",
      "LHER3",
      "", "",
      "AHEB6",
      "",
      "ENGI4",
      "", "", "",
      "NORD3",
      "ENGI3",
      "TIET4",
      "",
      "CGRA4",
      "FDMO34",
      "",
      "TIET3",
      "",
      "CGRA3",
      "CEBR3",
      "", "", "", "", "", "", "",
      "SOND6",
      "CARD3",
      "", "", "", "", "",
      "KMBB34",
      "", "", "", "",
      "MRSA3B",
      "", "", "", "", "", "", "", "", "",
      "ACNB34",
      "", "", "",
      "WHRL4",
      "SLED4",
      "", "", "",
      "WHRL3",
      "SLED3",
      "EZTC3",
      "", "", "",
      "CRTE3B",
      "EGIE3",
      "", "",
      "TCNO4",
      "", "", "", "",
      "TCNO3",
      "AALL34",
      "",
      "ENMA3B",
      "", "", "", "", "", "", "", "", "",
      "",
      "CEEB5",
      "",
      "IDNT3",
      "",
      "EMBR3",
      "", "", "", "", "", "",
      "CEPE3",
      "", "", "", "", "",
      "CORR4",
      "", "", "", "",
      "CORR3",
      "", "",
      "TAXA6",
      "JOPA4",
      "", "", "", "",
      "JOPA3",
      "", "", "", "", "", "", "", "", "",
      "CLSC4",
      "",
      "FIBR3",
      "", "",
      "CLSC3",
      "", "", "", "",
      "MERC4",
      "", "", "", "",
      "MERC3",
      "", "", "", "", "",
      "CAIA3B",
      "", "", "", "", "", "", "", "", "",
      "MCDC34",
      "", "", "", "", "", "",
      "ENBR3",
      "",
      "OMGE3",
      "", "", "", "",
      "COCE3",
      "BMEB4",
      "",
      "DIRR3",
      "",
      "CALI4",
      "BMEB3",
      "", "", "",
      "CALI3",
      "CRIV4",
      "", "", "",
      "TOYB4",
      "CRIV3",
      "", "", "",
      "TOYB3",
      "BMEB9",
      "", "", "",
      "RSUL4",
      "",
      "RIGG34",
      "", "",
      "RSUL3",
      "COCA34",
      "", "",
      "PINE4",
      "", "", "", "",
      "PINE3",
      "", "", "", "", "",
      "MAGG3",
      "",
      "BEEF3",
      "", "",
      "CESP3",
      "", "", "", "",
      "NEMO6",
      "QUSW3",
      "", "", "", "",
      "IGBR3",
      "CPFE3",
      "", "",
      "IDVL4",
      "DOHL4",
      "", "", "",
      "IDVL3",
      "DOHL3",
      "LIGT3",
      "NAFG4",
      "ECOR3",
      "", "", "",
      "NAFG3",
      "",
      "SEDU3",
      "LINX3",
      "", "", "", "",
      "SULA4",
      "",
      "NUTR3",
      "", "",
      "SULA3",
      "CANT4B",
      "", "",
      "FJTA4",
      "",
      "CANT3B",
      "", "",
      "FJTA3",
      "", "",
      "ENMT4",
      "", "", "", "",
      "ENMT3",
      "",
      "CSNA3",
      "CBEE3",
      "",
      "MEND5",
      "", "",
      "ARNC34",
      "",
      "ALEF3B",
      "",
      "CEBR6",
      "CABI3B",
      "", "", "",
      "TRPL4",
      "GOGL34",
      "ITUB4",
      "", "",
      "TRPL3",
      "",
      "ITUB3",
      "", "", "",
      "MYPK3",
      "CTKA4",
      "", "",
      "MRSA6B",
      "IRBR3",
      "CTKA3",
      "",
      "AHEB5",
      "", "",
      "CTSA4",
      "", "", "", "",
      "CTSA3",
      "", "",
      "LPSB3",
      "",
      "HOOT4",
      "", "", "",
      "SBUB34",
      "HOOT3",
      "", "",
      "LEVE3",
      "AAPL34",
      "MTSA4",
      "SOND5",
      "",
      "ALSC3",
      "",
      "MTSA3",
      "",
      "ELET3",
      "",
      "INEP4",
      "ENMA6B",
      "", "", "",
      "INEP3",
      "", "", "", "",
      "CATP34",
      "",
      "TWTR34",
      "", "",
      "NATU3",
      "CTSA8",
      "", "",
      "FRIO3",
      "FCXO34",
      "ITSA4",
      "TKNO4",
      "CEPE6",
      "CCRO3",
      "",
      "ITSA3",
      "TKNO3",
      "", "", "", "", "", "",
      "CCXC3",
      "",
      "WUNI34",
      "", "",
      "KEPL3",
      "COLG34",
      "", "", "", "", "", "", "", "", "",
      "AALR3",
      "",
      "STKF3",
      "", "",
      "CREM3",
      "MRFG3",
      "EALT4",
      "",
      "CMIG4",
      "", "",
      "EALT3",
      "",
      "CMIG3",
      "", "", "", "", "",
      "TIMP3",
      "", "", "",
      "AGRO3",
      "", "",
      "TAXA5",
      "",
      "MMXM3",
      "MEAL3",
      "",
      "POSI3",
      "", "",
      "COTY34",
      "", "", "",
      "MRVE3",
      "",
      "ELPL3",
      "", "", "", "",
      "ODER4",
      "",
      "COCE6",
      "",
      "RLOG3",
      "ODER3",
      "", "",
      "CNSY3",
      "",
      "FBOK34",
      "", "",
      "OFSA3",
      "", "", "", "", "", "", "",
      "MWET4",
      "", "", "",
      "BOEI34",
      "MWET3",
      "",
      "MOVI3",
      "IBMB34",
      "", "", "", "", "", "", "", "",
      "CPRE3",
      "", "", "", "",
      "BRGE3",
      "", "", "", "",
      "CPLE3",
      "", "", "",
      "CESP6",
      "",
      "MNPR3",
      "", "", "",
      "BRGE7",
      "KHCB34",
      "",
      "EMAE4",
      "", "",
      "HALI34",
      "",
      "EMAE3",
      "", "",
      "MRCK34",
      "",
      "POMO4",
      "PETR4",
      "BDLL4",
      "", "",
      "POMO3",
      "PETR3",
      "BDLL3",
      "", "", "", "",
      "BRGE8",
      "", "", "", "",
      "CELP3",
      "SAPR4",
      "", "", "",
      "SNSY3",
      "SAPR3",
      "BIIB34",
      "",
      "NEMO5",
      "TMOS34",
      "", "", "",
      "PDGR3",
      "CELP7",
      "HYPE3",
      "", "", "",
      "GOLL4",
      "",
      "GEOO34",
      "TOTS3",
      "", "",
      "BERK34",
      "",
      "ATTB34",
      "",
      "BRML3",
      "CASN4",
      "", "", "", "",
      "CASN3",
      "",
      "PATI4",
      "", "",
      "TSLA34",
      "",
      "PATI3",
      "",
      "SLCE3",
      "EKTR4",
      "", "", "", "",
      "EKTR3",
      "KROT3",
      "EUCA4",
      "", "",
      "ANDG4B",
      "ARMT34",
      "EUCA3",
      "",
      "HGTX3",
      "ANDG3B",
      "", "", "", "", "",
      "ABEV3",
      "", "", "", "", "",
      "SSFO34",
      "CEBR5",
      "TAXA1",
      "LUPA3",
      "LIPR3",
      "VISA34",
      "", "", "", "", "", "", "",
      "RAPT4",
      "",
      "ELET6",
      "", "",
      "RAPT3",
      "MULT3",
      "",
      "MRSA5B",
      "",
      "MAPT4",
      "", "", "", "",
      "MAPT3",
      "",
      "EQTL3",
      "",
      "GFSA3",
      "JPMC34",
      "",
      "ITEC3",
      "",
      "ALPA4",
      "", "",
      "NRTQ3",
      "LTEL3B",
      "ALPA3",
      "ETER3",
      "OIBR4",
      "",
      "CRTE5B",
      "", "",
      "OIBR3",
      "PARD3",
      "", "", "", "", "", "", "",
      "ENMA5B",
      "",
      "SUZB3",
      "", "",
      "HCBR3",
      "",
      "ESTR4",
      "",
      "MGLU3",
      "BOBR4",
      "",
      "ESTR3",
      "", "",
      "BOBR3",
      "ESTC3",
      "", "", "",
      "SLBG34",
      "",
      "CEPE5",
      "",
      "GRND3",
      "CACO3B",
      "", "", "", "",
      "QGEP3",
      "", "",
      "COPH34",
      "",
      "CHVX34",
      "", "", "", "",
      "XRXB34",
      "", "",
      "PEPB34",
      "",
      "TRVC34",
      "NFLX34",
      "", "",
      "RSID3",
      "LOGN3",
      "", "", "",
      "ALUP4",
      "CCPR3",
      "",
      "EXXO34",
      "",
      "ALUP3",
      "JCPC34",
      "", "", "", "", "",
      "ARZZ3",
      "AMZO34",
      "", "",
      "BRSR3",
      "",
      "DEAI34",
      "", "", "", "", "", "",
      "AELP3",
      "",
      "MOAR3",
      "", "", "", "", "", "", "",
      "APTI4",
      "", "", "", "",
      "APTI3",
      "",
      "MSBR34",
      "COCE5",
      "BRGE6",
      "", "", "", "",
      "CPLE6",
      "",
      "SCHW34",
      "", "",
      "MSFT34",
      "ANIM3",
      "BNBR3",
      "",
      "ECPR4",
      "PFIZ34",
      "",
      "WALM34",
      "AMGN34",
      "ECPR3",
      "", "", "", "", "", "",
      "MSRO3",
      "", "", "", "", "", "",
      "GILD34",
      "", "",
      "FBMC4",
      "", "", "",
      "DTEX3",
      "FBMC3",
      "",
      "ABBV34",
      "",
      "CELP6",
      "", "",
      "COWC34",
      "CESP5",
      "SNSY6",
      "GRUC3",
      "", "", "", "", "",
      "AFLT3",
      "VERZ34",
      "", "", "",
      "SSBR3",
      "JFEN3",
      "", "", "", "", "", "", "", "", "",
      "GDBR34",
      "BAUH4",
      "", "",
      "ODPV3",
      "QCOM34",
      "BAUH3",
      "", "", "", "", "", "", "", "", "",
      "PNVL4",
      "", "",
      "BRIV4",
      "",
      "PNVL3",
      "",
      "SPRI3",
      "BRIV3",
      "", "", "",
      "MSPA4",
      "", "", "", "",
      "MSPA3",
      "",
      "CLGN34",
      "", "", "",
      "LUXM4",
      "", "", "", "",
      "LUXM3",
      "",
      "VLOE34",
      "MILS3",
      "",
      "BRDT3",
      "", "", "",
      "GEPA4",
      "", "", "",
      "GOGL35",
      "GEPA3",
      "BMYB34",
      "LILY34",
      "", "", "",
      "VALE3",
      "",
      "PSSA3",
      "", "",
      "CTSH34",
      "",
      "SMLS3",
      "", "", "",
      "FSLR34",
      "MNZC3B",
      "",
      "RPMG3",
      "",
      "LREN3",
      "", "", "", "", "", "", "", "", "",
      "BIOM3",
      "", "",
      "JBDU4",
      "", "", "", "",
      "JBDU3",
      "",
      "ELET5",
      "PRIO3",
      "", "", "",
      "CTNM4",
      "", "", "",
      "GGBR4",
      "CTNM3",
      "", "", "",
      "GGBR3",
      "HBOR3",
      "", "",
      "UGPA3",
      "",
      "AMAR3",
      "ITLC34",
      "",
      "BBSE3",
      "", "", "", "",
      "CSMG3",
      "AXPB34",
      "", "", "", "", "", "", "", "", "",
      "WFCO34",
      "", "", "", "", "", "", "", "", "",
      "ORCL34",
      "CTCA3",
      "", "",
      "TXRX4",
      "OPSE3B",
      "", "", "",
      "TXRX3",
      "", "", "", "", "", "",
      "CTCA9",
      "", "", "", "",
      "BTTL4",
      "", "",
      "BRSR6",
      "",
      "BTTL3",
      "", "", "", "",
      "BSEV3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "ABTT34",
      "", "", "", "",
      "QUAL3",
      "BRQB3",
      "",
      "FLRY3",
      "BONY34",
      "", "",
      "PEAB4",
      "OSXB3",
      "",
      "BAZA3",
      "CTGP34",
      "PEAB3",
      "", "", "",
      "TRIS3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "BRKM3",
      "MSCD34",
      "", "", "",
      "SBSP3",
      "", "",
      "BRGE5",
      "", "", "", "",
      "CPLE5",
      "", "", "", "",
      "BTOW3",
      "GRUC6",
      "", "", "", "", "",
      "PTBL3",
      "", "", "", "", "", "", "", "", "",
      "ETRO3B",
      "BNPA3B",
      "", "",
      "HPQB34",
      "",
      "MACY34",
      "",
      "ESRX34",
      "", "", "",
      "TAEE11",
      "", "",
      "JSLG3",
      "BOXP34",
      "",
      "CELP5",
      "", "", "", "",
      "SNSY5",
      "", "",
      "CSCO34",
      "CVSH34",
      "",
      "SPRI6",
      "ABCB4",
      "CSAB4",
      "", "", "", "",
      "CSAB3",
      "", "", "", "", "",
      "SCAR3",
      "", "", "", "",
      "IMOB11",
      "", "", "", "", "", "", "", "",
      "TECN3",
      "",
      "CVCB3",
      "BEES4",
      "", "", "", "",
      "BEES3",
      "MPLU3",
      "", "",
      "BMKS3",
      "CMCS34",
      "", "", "", "", "",
      "ADHM3",
      "CPTP3B",
      "", "",
      "CRPG6",
      "",
      "GMCO34",
      "",
      "ATOM3",
      "", "",
      "BETP3B",
      "MTIG4",
      "", "", "", "",
      "MTIG3",
      "", "",
      "BALM4",
      "", "", "",
      "PGCO34",
      "BALM3",
      "SANB11",
      "", "", "",
      "OGXP3",
      "", "", "", "", "", "", "",
      "MMAQ4",
      "", "", "", "",
      "MMAQ3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "LTEL5B",
      "", "", "",
      "GPSI34",
      "RNEW11",
      "", "",
      "BBDC4",
      "",
      "SPRT3B",
      "",
      "NEWT3B",
      "BBDC3",
      "TUPY3",
      "", "",
      "UTEC34",
      "",
      "RPAD3",
      "SLCT3B",
      "", "", "", "", "", "", "", "", "",
      "",
      "JBSS3",
      "", "", "", "", "", "", "",
      "BRGE12",
      "", "", "", "", "",
      "STBP3",
      "", "",
      "UNIP3",
      "", "", "", "", "", "", "",
      "LLIS3",
      "", "", "", "", "", "",
      "BAHI3",
      "VLID3",
      "", "", "", "", "", "", "", "", "",
      "BRPR3",
      "", "", "", "",
      "FNCN3",
      "", "", "", "",
      "TRPN3",
      "", "",
      "BRSR5",
      "BBRK3",
      "", "",
      "SGPS3",
      "", "", "", "", "",
      "FTRT3B",
      "",
      "KLBN4",
      "FRRN3B",
      "", "", "",
      "KLBN3",
      "", "",
      "BRKM6",
      "", "", "", "", "", "",
      "GSGI34",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "BGIP4",
      "", "", "", "",
      "BGIP3",
      "",
      "GPIV33",
      "", "", "", "", "", "", "", "",
      "PTNT4",
      "", "", "", "",
      "PTNT3",
      "", "", "", "", "", "", "", "", "",
      "",
      "BOAC34",
      "", "", "",
      "USBC34",
      "", "", "", "",
      "USSX34",
      "",
      "PFRM3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "",
      "PSEG4",
      "", "", "", "",
      "PSEG3",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "SMLL11",
      "",
      "SPRI5",
      "", "", "", "", "", "", "", "",
      "VULC3",
      "", "", "", "",
      "ICO211",
      "", "", "", "", "",
      "TPIS3",
      "",
      "BRFS3",
      "", "", "", "", "",
      "GPRO34",
      "", "", "", "", "", "", "", "", "",
      "",
      "BLAK34",
      "", "", "", "", "", "", "", "", "",
      "",
      "CRPG5",
      "", "",
      "BMIN4",
      "", "", "", "",
      "BMIN3",
      "", "", "",
      "BSLI4",
      "", "", "", "",
      "BSLI3",
      "UBSG34",
      "", "", "", "", "", "",
      "WSON33",
      "", "", "", "",
      "VIVR3",
      "",
      "RPAD6",
      "CSRN3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "",
      "SQRM3",
      "SPRN34",
      "ENGI11",
      "UNIP6",
      "",
      "WIZS3",
      "", "",
      "TIET11",
      "", "",
      "EVEN3",
      "", "", "",
      "LCAM3",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "CGAS3",
      "", "", "", "", "", "", "",
      "IDIV11",
      "", "", "", "", "", "", "", "",
      "BFRE12",
      "", "", "", "", "", "", "", "", "",
      "FRRN6B",
      "", "", "", "", "", "", "", "", "",
      "",
      "IEEX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "",
      "BPHA3",
      "", "", "", "", "", "", "", "", "",
      "PRPT3B",
      "", "", "", "", "", "", "",
      "BRKM5",
      "",
      "IVPR4B",
      "", "", "", "",
      "IVPR3B",
      "", "",
      "HBTS3",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "OPGM3B",
      "",
      "VIVT4",
      "",
      "PCAR4",
      "", "",
      "VIVT3",
      "",
      "PCAR3",
      "", "", "", "", "", "", "", "", "",
      "",
      "INDX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "",
      "PMAM3",
      "", "", "", "",
      "FRAS3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "BRIN3",
      "LBRN34",
      "", "", "",
      "OPHE3B",
      "", "", "", "",
      "IFNC11",
      "", "", "", "",
      "UPKP3B",
      "", "", "",
      "PRMN3B",
      "AVON34",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "DTCY4",
      "",
      "CSRN6",
      "", "",
      "DTCY3",
      "", "", "",
      "SULA11",
      "GUAR4",
      "", "", "", "",
      "GUAR3",
      "", "", "", "", "",
      "UPSS34",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "ISEE11",
      "", "", "", "", "", "",
      "RPAD5",
      "",
      "GOAU4",
      "", "", "", "",
      "GOAU3",
      "", "", "", "", "", "", "", "",
      "USIM3",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "UNIP5",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "",
      "FRRN5B",
      "", "", "", "", "", "", "", "", "",
      "",
      "IGNM11",
      "", "", "", "", "",
      "HBTS6",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "LTEL11B",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "PPAR3",
      "", "", "", "", "", "", "",
      "IBOV11",
      "", "",
      "BRGE11",
      "", "", "", "", "", "", "",
      "OPTS3B",
      "", "", "", "", "", "",
      "BRAP4",
      "", "", "", "",
      "BRAP3",
      "", "", "", "", "",
      "UPAC34",
      "", "", "", "",
      "BPAR3",
      "", "", "",
      "BPAC3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "",
      "IBXL11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "EBAY34",
      "", "", "", "",
      "CSRN5",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "USIM6",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "CSAN3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "GPAR3",
      "", "", "", "", "", "", "", "",
      "HSHY34",
      "CGAS5",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "IBRA11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "BPAT33",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "HBTS5",
      "", "", "", "",
      "IFIX11",
      "", "", "", "", "", "", "", "", "",
      "ALUP11",
      "", "", "", "", "",
      "VSPT4",
      "", "", "", "",
      "VSPT3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "GPCP3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "",
      "PLAS3",
      "", "", "",
      "BFRE11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "IGCT11",
      "",
      "UCAS3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "USIM5",
      "", "",
      "IGCX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "GSHP3",
      "", "", "", "", "", "", "",
      "ICON11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "IMAT11",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "BBYY34",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "MLCX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "BDRX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "IBXX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "BPAC5",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "VVAR4",
      "", "", "", "",
      "VVAR3",
      "", "", "", "",
      "BDRU11",
      "", "", "", "", "",
      "BPAN4",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "",
      "UTIL11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "BSAN33",
      "IVBX11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "",
      "BBAS3",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "KLBN11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "SQRM11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "",
      "ITAG11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "",
      "BPAC11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "BBAS12",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "VVAR11",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "",
      "BBAS11"
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key];

          if (*str == *s && !strcmp (str + 1, s + 1))
            return s;
        }
    }
  return 0;
}
