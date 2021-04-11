//Notes are all in 5th (LOW) & 6th octave (Soprano)
#define LOWCNOTE 523
#define LOWCSHARP 554
#define LOWDNOTE 587
#define LOWDSHARP 622
#define LOWENOTE 659
#define LOWFNOTE 698
#define LOWFSHARP 740
#define LOWGNOTE 784
#define LOWGSHARP 830
#define LOWANOTE 880
#define LOWASHARP 932
#define LOWBNOTE 988
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
#define THREECOUNT 900
#define SEMIBREEVE 1200
#define DOUBLESEMIBREEVE 2400

#define BREAK 25

#define SCALE_NOTE_COUNT 13
#define BLINDING_LIGHTS_NOTE_COUNT 46
#define TAKE_ON_ME_NOTE_COUNT 48
#define NOKIA_THEME_NOTE_COUNT 10


//Musical note target frequencies
int scale[] = {LOWCNOTE, LOWCSHARP, LOWDNOTE, LOWDSHARP, LOWENOTE, LOWFNOTE, LOWFSHARP, LOWGNOTE, LOWGSHARP, LOWANOTE, LOWASHARP, LOWBNOTE, CNOTE};

//Nokia Theme
int nokiaTheme[] = {
										GSHARP, BLANK,
										FSHARP, BLANK,
										LOWASHARP, BLANK,
										CSHARP, BLANK,
										FSHARP, BLANK,
									 };

int nokiaThemeTempo[] = {
													QUAVER, BREAK,
													QUAVER, BREAK,
													CROTCHET, BREAK,
													CROTCHET, BREAK,
													MINIM, BREAK
												};

//Take-on-me riff
int takeOnMe[] = {
									FSHARP, BLANK,
									FSHARP, BLANK,
									DNOTE, BLANK,
									LOWBNOTE, BLANK,
									LOWBNOTE, BLANK,
									ENOTE, BLANK,
									ENOTE, BLANK,
									ENOTE, BLANK,
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
												};


