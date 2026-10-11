FROM golang:1.27-alpine AS builder
WORKDIR /app
RUN apk add --no-cache gcc musl-dev
COPY go.mod go.sum ./
COPY . .
RUN CGO_ENABLED=1 GOOS=linux go build -o server ./cmd/server
RUN CGO_ENABLED=0 go install -tags='no_postgres no_clickhouse no_mssql no_mysql no_ydb no_azuresql no_vertica no_libsql' \
    github.com/pressly/goose/v3/cmd/goose@v3.26.0

FROM alpine:3.24 AS final
RUN apk --no-cache add ca-certificates
WORKDIR /root/
COPY --from=builder /app/server ./server
COPY --from=builder /go/bin/goose ./goose
COPY --from=builder /app/migrations ./migrations

EXPOSE 8000
CMD ["./server"]