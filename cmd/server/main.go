package main

import (
	"log"
	"meet_place/internal/ws"
	"net/http"
)

func main() {
	router := http.NewServeMux()

	router.HandleFunc("GET /ws", ws.WsHandler)
	
	log.Println("server started")
	err := http.ListenAndServe(":8000", router)
    if err != nil {
        log.Fatal(err)
    }
}
