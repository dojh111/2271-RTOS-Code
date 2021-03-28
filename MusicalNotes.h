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
#define TAKE_ON_ME_NOTE_COUNT 50


//Musical note target frequencies
int scale[] = {CNOTE, CSHARP, DNOTE, DSHARP, ENOTE, FNOTE, FSHARP, GNOTE, GSHARP, ANOTE, ASHARP, BNOTE, HIGHCNOTE};

//Take-on-me riff
int takeOnMe[] = {
									FSHARP, BLANK,
									FSHARP, BLANK,
									DNOTE, BLANK,
									BNOTE, BLANK, //Need change to the lower BNOTE
									BNOTE, BLANK,
									ENOTE, BLANK,
									ENOTE, BLANK,
									ENOTE, BLANK,
									GSHARP, BLANK,
									GSHARP, BLANK,
									GSHARP, BLANK,
									ANOTE, BLANK,
									BNOTE, BLANK,
									ANOTE, BLANK,
									ANOTE, BLANK,
									ANOTE, BLANK,
									ENOTE, BLANK,
									DNOTE, BLANK,
									FSHARP, BLANK,
									FSHARP, BLANK,
									FSHARP, BLANK,
									ENOTE, BLANK,
									ENOTE, BLANK,
									FSHARP, BLANK,
									ENOTE, BLANK
								 };

int takeOnMeTempo[] = {
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, QUAVER,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
												QUAVER, BREAK,
											};

//Blinding Lights riff
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

int blindingLightsTempo[] = 
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


