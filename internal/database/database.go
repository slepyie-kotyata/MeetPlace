package database

import (
	"context"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/clause"
)

type Database struct {
	dbConn	*gorm.DB
}

func seedTestUsers(db *gorm.DB) error {
    testUsers := []User{
        {Id: "user1", UserName: "TestUser1", Password: "abcd1"},
        {Id: "user2", UserName: "TestUser2", Password: "abcd2"},
        {Id: "user3", UserName: "TestUser3", Password: "abcd3"},
        {Id: "user4", UserName: "TestUser4", Password: "abcd4"},
    }

    return db.Clauses(clause.OnConflict{
        Columns: []clause.Column{{Name: "id"}},
        DoNothing: true,
    }).Create(&testUsers).Error
}

func NewDatabase() (*Database, error) {
	db, err := gorm.Open(sqlite.Open("data/users.db"), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	db.AutoMigrate(&User{})
	if err := seedTestUsers(db); err != nil {
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