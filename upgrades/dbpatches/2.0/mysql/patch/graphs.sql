ALTER TABLE graphs MODIFY graphid bigint unsigned NOT NULL,
		   MODIFY templateid bigint unsigned NULL,
		   MODIFY ymin_itemid bigint unsigned NULL,
		   MODIFY ymax_itemid bigint unsigned NULL;
UPDATE graphs SET templateid=NULL WHERE templateid=0;
CREATE TEMPORARY TABLE tmp_graphs_graphid (graphid bigint unsigned PRIMARY KEY);
INSERT INTO tmp_graphs_graphid (graphid) (SELECT graphid FROM graphs);
UPDATE graphs SET templateid=NULL WHERE NOT templateid IS NULL AND NOT templateid IN (SELECT graphid FROM tmp_graphs_graphid);
DROP TABLE tmp_graphs_graphid;
UPDATE graphs SET ymin_itemid=NULL WHERE ymin_itemid=0;
UPDATE graphs SET ymax_itemid=NULL WHERE ymax_itemid=0;
ALTER TABLE graphs ADD CONSTRAINT c_graphs_1 FOREIGN KEY (templateid) REFERENCES graphs (graphid) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE graphs ADD CONSTRAINT c_graphs_2 FOREIGN KEY (ymin_itemid) REFERENCES items (itemid) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE graphs ADD CONSTRAINT c_graphs_3 FOREIGN KEY (ymax_itemid) REFERENCES items (itemid) ON UPDATE CASCADE ON DELETE CASCADE;
