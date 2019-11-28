INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('table', 'mentor', 'mentor', 5, 'CREATE TABLE "mentor"
(
	uid varchar(20) not null
		constraint mentor_pk
			primary key,
	first_name varchar(30) not null,
	last_name varchar(30) not null,
	gender varchar(20) not null,
	academic_level varchar(20) not null,
	college varchar(50) not null,
	degree varchar(50) not null,
	type varchar(20) not null,
	languages varchar(50) not null,
	hall varchar(30) not null,
	special text,
	interests text,
	wwvp varchar(30) not null,
	train_1 char(1) default ''n'' not null,
	train_2 char(1) default ''n'' not null,
	train_3 char(1) default ''n'' not null,
	train_complete char(1) default ''n'' not null,
	round_1 char(1) default ''n'' not null,
	round_2 char(1) default ''n'' not null
)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('index', 'sqlite_autoindex_mentor_1', 'mentor', 6, null);
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('index', 'mentor_uid_uindex', 'mentor', 2, 'CREATE UNIQUE INDEX mentor_uid_uindex
	on mentor (uid)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('table', 'mentee', 'mentee', 3, 'CREATE TABLE mentee
(
	uid varchar(10) not null
		constraint mentee_pk
			primary key,
	first_name varchar(20) not null,
	last_name varchar(20) not null,
	academic_level varchar(10) not null,
	college varchar(50) not null,
	requests text,
	round_1 char(1) default ''n'' not null,
	round_2 char(1) default ''n'' not null
)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('index', 'sqlite_autoindex_mentee_1', 'mentee', 4, null);
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('index', 'mentee_uid_uindex', 'mentee', 7, 'CREATE UNIQUE INDEX mentee_uid_uindex
	on mentee (uid)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('table', 'match', 'match', 8, 'CREATE TABLE match
(
	m_id integer not null
		constraint match_pk
			primary key autoincrement,
	mentor_id varchar(20) not null,
	mentee_id varchar(20) not null
)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('table', 'sqlite_sequence', 'sqlite_sequence', 9, 'CREATE TABLE sqlite_sequence(name,seq)');
INSERT INTO sqlite_master (type, name, tbl_name, rootpage, sql) VALUES ('index', 'match_m_id_uindex', 'match', 10, 'CREATE UNIQUE INDEX match_m_id_uindex
	on match (m_id)');
