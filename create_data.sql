CREATE TABLE IF NOT EXISTS usuario (
    email TEXT,
    token TEXT
)
#EOQ#
CREATE TABLE IF NOT EXISTS nota (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    resource_id TEXT DEFAULT "",
    texto TEXT,
    data_hora UNIXEPOCH DEFAULT (strftime('%s','now'))
)
#EOQ#
CREATE TABLE IF NOT EXISTS info_sync_nota (
    local_id INTEGER,
    resource_id TEXT,
    tipo_sync TEXT,
    CONSTRAINT sync_pkey PRIMARY KEY (local_id,resource_id, tipo_sync)
)
#EOQ#
CREATE TRIGGER IF NOT EXISTS sync_update AFTER UPDATE OF texto ON nota
BEGIN
UPDATE nota SET data_hora = strftime('%s','now') WHERE id = OLD.id;
INSERT OR REPLACE INTO info_sync_nota (local_id, resource_id, tipo_sync) VALUES (OLD.id, OLD.resource_id, 'update');
END
#EOQ#
CREATE TRIGGER IF NOT EXISTS sync_delete AFTER DELETE ON nota
BEGIN
INSERT INTO info_sync_nota (local_id, resource_id, tipo_sync) VALUES (OLD.id, OLD.resource_id, 'delete');
END
#EOQ#
CREATE TABLE IF NOT EXISTS properties (
    key TEXT PRIMARY KEY,
    value  TEXT
)
#EOQ#
INSERT INTO properties (key, value) VALUES('db_version',1);
