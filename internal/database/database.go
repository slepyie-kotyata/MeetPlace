package database

import (
	"context"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

type Database struct {
	dbConn	*gorm.DB
}

func NewDatabase() (*Database, error) {
	db, err := gorm.Open(sqlite.Open("data/users.db"), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	return &Database{
		dbConn: db,
	}, nil
}

type User struct {
	Id			string	`gorm:"primaryKey"`
	UserName	string	`gorm:"not null"`
	Password	string	`gorm:"not null"`
}

func (d *Database) GetUserName(ctx context.Context, userId string) (string, error) {
	user, err := gorm.G[User](d.dbConn).Where("id = ?", userId).First(ctx)
	if err != nil {
		return "", err
	}
	return user.UserName, nil
}