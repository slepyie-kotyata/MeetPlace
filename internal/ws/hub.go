package ws

import (
	"errors"
	"fmt"
	"sync"
)

// type Room struct {
// 	HostID	string
// 	Peers	map[string]*Peer
// }
//
// func NewRoom() *Room {
// 	return &Room{
// 		Peers: make(map[string]*Peer, 4),
// 	}
// }

type Hub struct {
	mu			sync.RWMutex
	Rooms		map[string]map[string]*Peer
}

func NewHub() *Hub {
	return &Hub{
		mu: sync.RWMutex{},
		Rooms: make(map[string]map[string]*Peer),
	}
}

var hub = NewHub()

func (h *Hub) HandleMessages(peer *Peer, message Message) {
	switch(message.Type){
		case Offer, Answer, Ice:
			defer h.mu.RUnlock()
			h.mu.RLock()
			target, ok := h.Rooms[peer.roomID][message.TargetID]
			if !ok {
				peer.outgoing <- Message{
					Type: Error,
					Code: PeerNotFound,
					Message: fmt.Sprintf("peer %s not found", message.TargetID),
				}
			}
			target.outgoing <- message
		case StateChanged:
			peer.Broadcast(message)
		default:
			peer.outgoing <- Message{
				Type: Error,
				Code: UnknownType,
				Message: fmt.Sprintf("Uknown command %s", message.Type),
			}
	}
}

func (h *Hub) Remove(peer *Peer) {
	defer h.mu.Unlock()
	h.mu.Lock()

	room, ok := h.Rooms[peer.roomID]
	if !ok {
		return
	}

	if _, exists := room[peer.id]; exists {
		delete(room, peer.id)
		if len(room) == 0 {
			delete(h.Rooms, peer.roomID)
		}
	}
}

func (h *Hub) Add(roomId string, peer *Peer) ([]PeerInfo, error) {
	defer h.mu.Unlock()
	h.mu.Lock()

	peers, ok := h.Rooms[roomId]
	if !ok {
		h.Rooms[roomId] = make(map[string]*Peer, 4)
		h.Rooms[roomId][peer.id] = peer
		return []PeerInfo{}, nil
	} else if len(peers) == 4 {
		return nil, errors.New("Room is full (max 4)")
	}

	var others []PeerInfo
	for _, peer := range peers {
		others = append(others, PeerInfo{PeerId: peer.id, Name: peer.name})
	}
	peers[peer.id] = peer
	return others, nil
}