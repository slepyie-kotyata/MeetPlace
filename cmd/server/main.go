package main

import (
	"log"
	"meet_place/internal/database"
	"meet_place/internal/ws"
	"net/http"
)

func main() {
	db, err := database.NewDatabase()
	if err != nil {
		log.Fatalf("Couldn't open database: %s", err.Error())
	}
	wsService := ws.NewWsService(db)
	
	router := http.NewServeMux()
	router.HandleFunc("GET /ws", wsService.WsHandler)
	
	log.Println("server started")
	err = http.ListenAndServe(":8000", router)
    if err != nil {
        log.Fatal(err)
    }
}
