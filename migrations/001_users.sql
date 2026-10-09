-- +goose Up
-- +goose StatementBegin
CREATE TABLE users (
    id TEXT PRIMARY KEY,
    username TEXT NOT NULL,
    password TEXT NOT NULL
);
INSERT INTO users (id, username, password) VALUES ('user1', 'TestUser1', 'abcd1');
INSERT INTO users (id, username, password) VALUES ('user2', 'TestUser2', 'abcd2');
INSERT INTO users (id, username, password) VALUES ('user3', 'TestUser3', 'abcd3');
INSERT INTO users (id, username, password) VALUES ('user4', 'TestUser4', 'abcd4');
-- +goose StatementEnd

-- +goose Down
-- +goose StatementBegin
DROP TABLE users;
-- +goose StatementEnd