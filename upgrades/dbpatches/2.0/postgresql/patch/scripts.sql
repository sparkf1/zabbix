ALTER TABLE ONLY scripts ALTER scriptid DROP DEFAULT,
			 ALTER usrgrpid DROP DEFAULT,
			 ALTER usrgrpid DROP NOT NULL,
			 ALTER groupid DROP DEFAULT,
			 ALTER groupid DROP NOT NULL;
UPDATE scripts SET usrgrpid=NULL WHERE usrgrpid=0;
UPDATE scripts SET groupid=NULL WHERE groupid=0;
DELETE FROM scripts WHERE NOT usrgrpid IS NULL AND NOT usrgrpid IN (SELECT usrgrpid FROM usrgrp);
DELETE FROM scripts WHERE NOT groupid IS NULL AND NOT groupid IN (SELECT groupid FROM groups);
ALTER TABLE ONLY scripts ADD CONSTRAINT c_scripts_1 FOREIGN KEY (usrgrpid) REFERENCES usrgrp (usrgrpid) ON UPDATE CASCADE;
ALTER TABLE ONLY scripts ADD CONSTRAINT c_scripts_2 FOREIGN KEY (groupid) REFERENCES groups (groupid) ON UPDATE CASCADE;
