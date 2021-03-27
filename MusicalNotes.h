//Notes are all in 6th octave (Soprano)
#define CNOTE 1047
#define CSHARP 1109
#define DNOTE 1175
#define DSHARP 1245
#define ENOTE 1319
#define FNOTE 1397
#define FSHARP 1480
#define GNOTE 1568
#define GSHARP 1661
#define ANOTE 1760
#define ASHARP 1865
#define BNOTE 1976
#define HIGHCNOTE 2093
#define BLANK 0

//Based on 194 BPM
#define QUAVER 150
#define CROTCHET 300
#define CROTCHETHALF 450
#define MINIM 600
#define SEMIBREEVE 1200
#define DOUBLESEMIBREEVE 2400

#define BREAK 25

#define SCALE_NOTE_COUNT 13
#define BLINDING_LIGHTS_NOTE_COUNT 47


//Musical note target frequencies
int scale[] = {CNOTE, CSHARP, DNOTE, DSHARP, ENOTE, FNOTE, FSHARP, GNOTE, GSHARP, ANOTE, ASHARP, BNOTE, HIGHCNOTE};

int blindingLights[] = {
												FNOTE, BLANK, 
												FNOTE, BLANK, 
												DSHARP, BLANK, 
												FNOTE, BLANK, 
												GNOTE, BLANK, 
												CNOTE, BLANK, 
												DSHARP, BLANK,
												FNOTE, BLANK,
												FNOTE, BLANK,
												DSHARP, BLANK,
												FNOTE, BLANK,
												GNOTE, BLANK,
												CNOTE, BLANK,
												DSHARP, BLANK,
												ASHARP, BLANK,
												GNOTE, BLANK,
												FNOTE, BLANK,
												DSHARP, BLANK,
												ASHARP, BLANK,
												GNOTE, BLANK,
												FNOTE, BLANK,
												DSHARP, BLANK,
												FNOTE, BLANK,
												//Pause
												BLANK
												};
int blindingLightsTemp[] = 
												{
													MINIM, BREAK,
													CROTCHETHALF, BREAK,
													QUAVER, BREAK,
													QUAVER, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													CROTCHETHALF, BREAK,
													MINIM, BREAK,
													CROTCHETHALF, BREAK,
													QUAVER, BREAK,
													QUAVER, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													CROTCHETHALF, BREAK,
													QUAVER, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													CROTCHETHALF, BREAK,
													QUAVER, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													DOUBLESEMIBREEVE, BREAK,
													//Pause
													CROTCHET
												};


