package ws

import (
	"errors"
	"sync"
)

type Hub struct {
	mu			sync.RWMutex
	Rooms		map[string]*Room
}

type Room struct {
	HostID	string
	Peers	map[string]*Peer
}

func NewHub() *Hub {
	return &Hub{
		mu: sync.RWMutex{},
		Rooms: make(map[string]*Room),
	}
}

func NewRoom() *Room {
	return &Room{
		Peers: make(map[string]*Peer, 4),
	}
}

var hub = NewHub()

//TODO: переделать существующие функции

func (h *Hub) Add(roomId string, peer *Peer) error {
	defer h.mu.Unlock()
	h.mu.Lock()

	room, ok := h.Rooms[roomId]
	if !ok {
		newRoom := NewRoom()
		newRoom.Peers[peer.Id], newRoom.HostID = peer, peer.Id
		h.Rooms[roomId] = newRoom
		return nil
	} else if len(room.Peers) == 4 {
		return errors.New("Room is full (max 4)")
	}

	room.Peers[peer.Id] = peer
	return nil
}