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
	handler := http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
    log.Printf("HTTP request: method=%s path=%s remote=%s",
        r.Method, r.URL.Path, r.RemoteAddr)
	
    router.ServeHTTP(w, r)
	})
	
	log.Println("server started")
	err = http.ListenAndServe(":8000", handler)
    if err != nil {
        log.Fatal(err)
    }
}
