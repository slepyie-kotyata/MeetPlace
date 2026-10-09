package ws

import (
	"encoding/json"
	"log"
	"meet_place/internal/database"
	"net/http"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	ReadBufferSize: 1024,
	WriteBufferSize: 1024,
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

//TODO: возможно перенести в /handlers

type WsService struct {
	dataBase *database.Database
}

func NewWsService(dataBase *database.Database) *WsService {
	return &WsService{
		dataBase: dataBase,
	}
}

func (ws *WsService) WsHandler(w http.ResponseWriter, r *http.Request) {
	peerId, roomId := r.URL.Query().Get("peer_id"), r.URL.Query().Get("room_id")
	peerName, err := ws.dataBase.GetUserName(r.Context(), peerId)
	if err != nil {
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusNotFound)
		json.NewEncoder(w).Encode(map[string]string{
			"message": "User not found",
		})
		return
	}

	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {		
		log.Printf("Upgrade error: %s", err.Error())
		return
	}
	peer := NewPeer(peerId, peerName, roomId, conn)
	
	if err = hub.Add(roomId, peer); err != nil {
		_ = conn.WriteJSON(Message{
			Type: Error,
			Code: RoomFull,
			Message: err.Error(),
		})
		conn.Close()
	}
	// запуск горутин Read и Write
	// отправление подтверждения joined и рассылка peer-joined остальным
	log.Println("client connected")	
}

