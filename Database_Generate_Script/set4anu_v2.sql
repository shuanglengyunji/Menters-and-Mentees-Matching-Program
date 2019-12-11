BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS [mentor] (
	group_id		INTEGER NOT NULL DEFAULT 0,
	is_confirmed 	CHAR(1) NOT NULL DEFAULT 'n',
	first_name		VARCHAR(20) NOT NULL,
	last_name		VARCHAR(20) NOT NULL,
	uid				VARCHAR(10) NOT NULL UNIQUE,
	wwvp			VARCHAR(10),
	round			INTEGER(1),
	academic_level	INTEGER(1),
	college			INTEGER(1),
	degree			VARCHAR(100),
	type			INTEGER(1),
	gender			INTEGER(1),
	languages		VARCHAR(50),
	languages_text	TEXT(500),
	hall			VARCHAR(50),
	special			VARCHAR(50),
	interests		TEXT(1000),
	train_1			CHAR(1) NOT NULL DEFAULT 'n',
	train_2			CHAR(1) NOT NULL DEFAULT 'n',
	train_3			CHAR(1) NOT NULL DEFAULT 'n',
	train_complete	CHAR(1) NOT NULL DEFAULT 'n',
	PRIMARY KEY("uid")
);
CREATE TABLE IF NOT EXISTS [mentee] (
	group_id			INTEGER NOT NULL DEFAULT 0,
	first_name			VARCHAR(20) NOT NULL,
	last_name			VARCHAR(20) NOT NULL,
	uid					VARCHAR(10) NOT NULL UNIQUE,
	round				INTEGER(1),
	academic_level		INTEGER(1),
	college				INTEGER(1),
	degree				VARCHAR(100),
	type				INTEGER(1),
	gender				INTEGER(1),
	languages			VARCHAR(50),
	languages_text		TEXT(500),
	special_categories	VARCHAR(50),
	requests			TEXT(1000),
	PRIMARY KEY("uid")
);
COMMIT;
