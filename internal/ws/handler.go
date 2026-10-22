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
	
	otherPeers, err := hub.Add(roomId, peer)
	if err != nil {
		_ = conn.WriteJSON(Message{
			Type: Error,
			Code: RoomFull,
			Message: err.Error(),
		})
		conn.Close()
	}

	go peer.Read()
	go peer.Write()

	peer.outgoing <- Message{
		Type: Joined,
		PeerID: peer.id,
		Peers: otherPeers,
	}
	
	peer.Broadcast(Message{
		Type: PeerJoined,
		PeerID: peer.id,
		Name: peer.name,
	})
	
	log.Println("client connected")	
}

