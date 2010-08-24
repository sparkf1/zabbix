ALTER TABLE ONLY profiles ALTER profileid DROP DEFAULT,
			  ALTER userid DROP DEFAULT;
DELETE FROM profiles WHERE NOT userid IN (SELECT userid FROM users);
ALTER TABLE ONLY profiles ADD CONSTRAINT c_profiles_1 FOREIGN KEY (userid) REFERENCES users (userid) ON UPDATE CASCADE ON DELETE CASCADE;
