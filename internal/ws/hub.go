package ws

import (
	"errors"
	"fmt"
	"sync"
)

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

func (h *Hub) Remove(peer *Peer) {
	defer h.mu.Unlock()
	h.mu.Lock()
	delete(h.Rooms[peer.RoomId], peer.Id)

	if len(h.Rooms[peer.RoomId]) == 0 {
		delete(h.Rooms, peer.RoomId)
	}
}

func (h *Hub) HandleRequestMessages(message Message, peer *Peer) {
	switch(message.Type){
	case Join:
		h.Join(message, peer)
		
	case Leave:
	default:
		peer.Outgoing <- Message{
			Type: Error,
			Code: UnknownType,
			Message: fmt.Sprintf("Uknown type: %s", message.Type),
		}
	}
}

func GetPeersInfo(peerId string, allPeers map[string]*Peer) []PeerInfo {
	defer mu.RUnlock()
	mu.RLock()
	
	var peerInfo []PeerInfo
	for id, _ := range allPeers {
		if id != peerId {
			peerInfo = append(peerInfo, PeerInfo{
				PeerId: id,
			})
		}
	}
	return peerInfo
}

func (h *Hub) Add(peerId, roomId string, peer *Peer) error {
	defer h.mu.Unlock()
	h.mu.Lock()

	peers, ok := h.Rooms[roomId]
	if !ok {
		h.Rooms[roomId = make(map[string]*Peer, 4)
	} else if len(peers) == 4 {
		return errors.New("Room is full (max 4)")
	}

	peer.Id, peer.RoomId = peerId, roomId
	h.Rooms[roomId][peerId] = peer

	return nil
}

func (h *Hub) Join(message Message, peer *Peer) {
	roomId, peerId := message.RoomID, message.PeerID
	
	if err := hub.Add(peerId, roomId, peer); err != nil {
		peer.Outgoing <- Message{
			Type: Error,
			Code: RoomFull,
			Message: err.Error(),
		}
		return
	}

	peer.Outgoing <- Message{
		Type: Joined,
		RoomID: roomId,
		PeerID: peerId,
		Peers: GetPeersInfo(peerId, hub.Rooms[roomId]),
	}

	defer h.mu.RUnlock()
	h.mu.RLock()
	for id, p := range h.Rooms[roomId]{
		if id != peerId {
			p.Outgoing <- Message{
				Type: PeerJoined,
				RoomID: roomId,
				PeerID: peerId,
			}
		}	
	}
	return
}