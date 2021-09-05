FROM gcc:4.9
COPY . ../FaceRecoginzer
WORKDIR ../FaceRecoginzer
EXPOSE 5555
RUN g++ -o Main Main.cpp -std=gnu++11
CMD ["./Main"]