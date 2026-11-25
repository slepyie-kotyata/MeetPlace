# syntax=docker/dockerfile:1
FROM golang:1.27-alpine AS builder
RUN apk add --no-cache build-base
WORKDIR /app
COPY go.mod go.sum ./
RUN --mount=type=cache,target=/go/pkg/mod go mod download
COPY cmd ./cmd
COPY internal ./internal
RUN --mount=type=cache,target=/go/pkg/mod \
    --mount=type=cache,target=/root/.cache/go-build \
    CGO_ENABLED=1 go build -ldflags="-s -w" -o server ./cmd/server

FROM alpine:3.22 AS final
RUN apk add --no-cache ca-certificates
WORKDIR /root/
COPY --from=builder /app/server ./server
EXPOSE 8000
CMD ["./server"]