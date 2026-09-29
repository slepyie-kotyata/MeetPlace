package ws

import (
	"sync"

	"github.com/gorilla/websocket"
)

const (
	Join = "join"
	Joined = "joined"
	PeerJoined = "peer-joined"
	Leave = "leave"
	Left = "left"
	PeerLeft = "peer-left"
	Offer = "offer"
	Answer = "answer"
	Ice = "ice"
	Error = "error"
)

type Message struct {
	Type		string			`json:"type"`
	RoomID		string			`json:"room_id,omitempty"`
	PeerID		string			`json:"peer_id,omitempty"`
	TargetID  	string			`json:"target_id,omitempty"`
	Name     	string			`json:"name,omitempty"`
	Payload		*WebRTCPayload	`json:"payload,omitempty"`
	Peers		[]PeerInfo		`json:"peers,omitempty"`
	Code    	string 			`json:"code,omitempty"`
	Message 	string 			`json:"message,omitempty"`
}

type WebRTCPayload struct {
	SDP           string `json:"sdp,omitempty"`
	Candidate     string `json:"candidate,omitempty"`
	SDPMid        string `json:"sdpMid,omitempty"`
	SDPMLineIndex *int   `json:"sdpMLineIndex,omitempty"` 
}

type PeerInfo struct {
	PeerId	string	`json:"peer_id"`
	Name	string	`json:"name"`
}

type Hub struct {
	mu			sync.RWMutex
	Peers 		map[string]*websocket.Conn
	Rooms		map[string][]PeerInfo
}